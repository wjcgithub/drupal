<?php

/**
 * Build the header of the result table.
 *
 * @param $user_page_user
 *    if we are on a user page, the user that page belongs to, not the current user
 *
 * @return array respecting _or_chart_tablesort_sql()
 */
function _or_chart_url_build_header($user_page_user = NULL) {

  $header = array();
  if (user_access('manage and filter site statistics')) { 
    //$header[] = theme('table_select_header_cell');
  }
    $header[] = array('data' => t('url'), 'field' => 'url');
    $header[] = array('data' => t('url名'), 'field' => 'urlname');
    $header[] = array('data' => t('时间'), 'field' => 'created', 'sort' => 'desc');
    $header[] = array('data' => t('总访问量'), 'field' => 'visit_num');
  if (user_access('manage and filter site statistics')) {
    $header[] = array('data' => t('操作'));
  }

  //add table select all
  drupal_add_js('misc/tableselect.js');
  array_unshift($header, array('class' => array('select-all')));
  return $header;
}
 
/**
 * Perform adjusted query.
 * 
 * @param $user_page_user
 *    if we are on a user page, the user that page belongs to, not the current user
 *
 *
 * @param array respecting _or_chart_tablesort_sql()
 * @return result of permormed query
 */
function _or_chart_url_perform_query($header, $kind = NULL, $user_page_user = NULL) { 
  //获取参数 where join args   
  $filter = _or_chart_build_filter_query();
  $where = ' '. $filter['where'];
    $query = "SELECT ucp.id, ucp.tid, ucp.url, ucp.url_humen, ucp.visit_num, ucp.created "
    ."FROM {url_counter_per} ucp "
    . $filter['join']
    . $where
    . _or_chart_tablesort_sql($header);
    $limit = isset($_SESSION['or_chart_max_rows']) ? $_SESSION['or_chart_max_rows'] : 50;
    $page = pager_default_initialize(db_query($query,$filter['args'])->rowCount(), $limit);
    $offset = $limit * $page; 
    $result = db_query_range($query, $offset, $limit, $filter['args']);
    return $result;
}

/**
 * Defines the form for nodes administration filter results.
 *
 * @param $user_page_user
 *    if we are on a user page, the user that page belongs to, not the current user
 *
 * @ingroup forms
 * _or_chart
 * @return array with forms properties
 */
function _or_chart_admin_url_form($form, $form_state) {   
  global $user;
  $destination = drupal_get_destination();   

  // Build an 'Update options' form.
  if (user_access('manage and filter site statistics')) {
    $form['options'] = array(
       '#type' => 'fieldset', 
       '#title' => t('可执行操作'),
       '#prefix' => '<div class="container-inline">', 
       '#suffix' => '</div>',
    );    
    $form['options']['export'] = array('#type' => 'submit', '#value' => t('导出数据'), '#submit' => array('_or_chart_admin_url_export_submit'));
  }

  // Load the nodes that we want to display.
  $form['header'] = array(
    '#type'  => 'value',
    '#value' => _or_chart_url_build_header(),
    );

  $result = _or_chart_url_perform_query($form['header']['#value'],  NULL);  
   
  // Build a table listing the appropriate nodes.
  $nodes = array();  
  while ($node = $result->fetchObject()) {
    $nodes[$node->id] = '';

    $form['url'][$node->id] = array('#type' => 'markup', '#markup' => l($node->url,$node->url));
    $form['urlname'][$node->id] = array('#type' => 'markup', '#markup' => $node->url_humen);
    $form['visit_num'][$node->id] = array('#type' => 'markup', '#markup' => $node->visit_num);
    $form['created'][$node->id] = array('#type' => 'markup', '#markup' =>date('Y-m-d',$node->created));

    if (user_access('statistics_url')) {
      // $form['operations'][$node->id][] = array('#type' => 'markup', '#markup' => l(_or_chart_get_img('count', t('统计')) .
      //   t('统计'), 'javascript:alert("开发中。。。")', array('query' => $destination, 'html' => TRUE)));
      $form['operations'][$node->id][] = array('#type' => 'markup', '#markup' =>_or_chart_get_img('count', t('统计')).t('统计'));
    }
  }
 
  if (user_access('manage and filter site statistics')) {
    $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  }
 
  $form['pager'] = array('#type'=>'markup', '#markup' => theme('pager')); 

  return $form;
}

/**
 * Form validation before submit.
 * We can't execute any 'Update options' if no nodes were selected.
 *
 * @ingroup forms
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */
function _or_chart_admin_url_form_validate($form, &$form_state) { 
  $nodes = array_filter($form_state['values']['nodes']);
  if (count($nodes) == 0) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form validation before submit.
 * We can't execute any 'Update options' if no nodes were selected.
 *
 * @ingroup forms
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */
function _or_chart_admin_url_export_validate($form, &$form_state) { 
  $nodes = array_filter($form_state['values']['nodes']);
  if (count($nodes) == 0) {
    form_set_error('', t('No items selected.'));
  }
}


/**
 * Handle post-validation form submission. \n
 * Execute the chosen 'Update option' on the selected nodes, such as
 * publishing, unpublishing, promotion and stickness status or deleting.
 *
 * @ingroup forms
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */ 
function _or_chart_admin_url_export_submit($form, &$form_state) {
  $nodes = array_filter($form_state['values']['nodes']);
  _or_chart_url_export_operation($form,$nodes);
}


/**
 * 导出到 Excel 实际操作
 * 
 * @param type $result 
 */
function _or_chart_url_export_operation($form,$result){
    $header = array();
    $data = array();
    foreach($form['header']['#value'] as $key=>$value){ 
        if(isset($value['field']) && !empty($value['field']))
        $header[$value['field']] = $value['data'];
    }
    
    //设置值
    foreach ($result as $key => $id) {
        $query = "SELECT ucp.id, ucp.tid, ucp.url, ucp.url_humen, ucp.visit_num, ucp.created "
        ."FROM {url_counter_per} ucp "
        .'where ucp.id='.$id; 
        $res = db_query($query)->fetchAll();
        if(isset($header['url']))
         $data[$key]['url'] = $res[0]->url;
         $data[$key]['urlname'] = $res[0]->url_humen;
         $data[$key]['created'] = date('Y-m-d',$res[0]->created);
         $data[$key]['visit_num'] = $res[0]->visit_num;
    }  
    $objPHPExcelc = custom_excel(variable_get('or_chart_phpexcel_path'),date('Y-m-d',time()).'骨科在线url统计报表');
    $objPHPExcelc->initHead($header)->initData($data);

    $objPHPExcel = $objPHPExcelc->getPhpExcelClass();
    //获取活动工作表
    $objActSheet = $objPHPExcel->getActiveSheet(); 
    //设置大标题
    $objPHPExcel->setActiveSheetIndex(0)->setCellValueExplicit('A1', date('Y-m-d',time()).'骨科在线url统计报表', PHPExcel_Cell_DataType::TYPE_STRING);
    //设置字体  
    $objStyleA1 = $objActSheet->getStyle('A1'); 
    $objStyleA1  
        ->getNumberFormat()  
        ->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER);  
    $objFontA1 = $objStyleA1->getFont();  
    $objFontA1->setName('Courier New');  
    $objFontA1->setSize(15);  
    $objFontA1->setBold(true);  
    $objFontA1->getColor()->setARGB('000000000'); 

    //设置当前活动sheet的名称  
    $objActSheet->setTitle('骨科在线统计报表'); 
    //设置宽度
    $objActSheet->getColumnDimension('A')->setAutoSize(true); 
    $objActSheet->getColumnDimension('B')->setAutoSize(true);  
    $objActSheet->getColumnDimension('C')->setAutoSize(true);  
    $objActSheet->getColumnDimension('D')->setAutoSize(true);  
//  $objActSheet->getColumnDimension('A')->setWidth(15);

    //设置全文剧中对齐
    $objStyleA5 = $objActSheet->getStyle('A1:I'.(count($data)+2));  
    $objAlignA5 = $objStyleA5->getAlignment();  
    $objAlignA5->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);  
    $objAlignA5->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);  

    //合并单元格
    $objActSheet->mergeCells('A1:I1');  

    $objStyleA5 = $objActSheet->getStyle('A1:I1');  
    //设置填充颜色  
    $objFillA5 = $objStyleA5->getFill();  
    $objFillA5->setFillType(PHPExcel_Style_Fill::FILL_SOLID);  
    $objFillA5->getStartColor()->setARGB('ee299229');

    //设置字体  
    $objStyleA5 = $objActSheet->getStyle('A2:I2'); 
    $objStyleA5  
        ->getNumberFormat()  
        ->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER);  
    $objFontA5 = $objStyleA5->getFont();  
    $objFontA5->setName('Courier New');  
    $objFontA5->setSize(13);  
    $objFontA5->setBold(true);  
//  $objFontA5->setUnderline(PHPExcel_Style_Font::UNDERLINE_SINGLE);  
    $objFontA5->getColor()->setARGB('000000000');  
    //取出
    $objPHPExcelc->execute(); exit;
}

/**
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results.
 */
function theme__or_chart_admin_url_form($form) {    //wjcPrint($form['form']);
  $form = $form['form'];
  $rows = array();
  $output = drupal_render($form['options']);  
  if (isset($form['url']) && is_array($form['url'])) {     // wjcPrint(element_children($form['title']) );
    foreach (element_children($form['url']) as $key) {
      $row = array();
      if (user_access('statistics_url')) {
        $row[] = drupal_render($form['nodes'][$key]);
      }
      //添加统计信息
      $row[] = drupal_render($form['url'][$key]);
      $row[] = drupal_render($form['urlname'][$key]);
      $row[] = drupal_render($form['created'][$key]);
      $row[] = drupal_render($form['visit_num'][$key]);  
      if (user_access('manage and filter site statistics')) {
        $row[] = drupal_render($form['operations'][$key]);
      }
      $rows[] = $row;
    }
    //wjcPrint($form['header']['#value']);
  } 
  else {
    $rows[] = array(array('data' => t('Filter returned no results.'), 'colspan' => '7'));
  }
  
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows)); 
  if ($form['pager']['#markup']) {
    $output .= render($form['pager']);
  }
  
   $output .= drupal_render_children($form);

  return $output;
}
