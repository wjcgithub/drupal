<?php

/**
 * @file
 * @brief Content management filter node operations file,
 *
 * This file contains all the node functions used by the module.
 *
 */
 

/**
 * Defines the form for nodes administration filter results.
 *
 * @param $user_page_user
 *    if we are on a user page, the user that page belongs to, not the current user
 *
 * @ingroup forms
 *
 * @return array with forms properties
 */
function cmf_heart_bottom_link_admin_nodes_form($form, $form_state) {
  global $user;
  $destination = drupal_get_destination();   
  $lang_codes = array('und' => t('Neutral'));
  if (module_exists('locale')) {
    $locale_available = TRUE;
    $lang_codes += locale_language_list('name'); 
  }
  else {
    $locale_available = FALSE;
  }

  // Build an 'Update options' form.
  if (user_access('administer nodes') && user_access('use_cmf_heart_bottom_link_operation')) {
    $form['options'] = array(
       '#type' => 'fieldset', 
       '#title' => t('Update options'),
       '#prefix' => '<div class="container-inline">', 
       '#suffix' => '</div>',
    );  
    $form['options']['operation'] = array(
      '#type' => 'select',
      '#options' => cmf_heart_bottom_link_admin_node_operations(),
      '#default_value' => 'approve',
      );
    $form['options']['submit'] = array('#type' => 'submit', '#value' => t('删除'), '#submit' => array('cmf_heart_bottom_link_admin_nodes_sort_form_submit'), '#validate'=>array('cmf_heart_bottom_link_admin_nodes_sort_form_validate'));
  }

  // Load the nodes that we want to display.
  $form['header'] = array(
    '#type'  => 'value',
    '#value' => cmf_heart_bottom_link_build_header(),
    );

  $result = cmf_heart_bottom_link_perform_query($form['header']['#value'],  NULL);  
   
  // Identify that the elements in 'example_items' are a collection, to
  // prevent Form API from flattening the array when submitted. 
  // 使提交的数据成数组结构
  $form['columnform']['#tree'] = TRUE;
  // Build a table listing the appropriate nodes.
  $nodes = array();  
  while ($node = $result->fetchObject()) {
    if(is_numeric($node->nid)){
      $nodes[$node->nid] = '';
      if ($_SESSION['cmf_heart_bottom_link_show_nid']) {
        $form['columnform'][$node->nid]['cmf_heart_bottom_link_id'] = array('#type' => 'markup', '#markup' => $node->nid);
      }

      // Don't mark it if it is the current user.
      $mark = $node->uid == $user->uid ? NULL: ' '. theme('mark', array('type'=>node_mark($node->nid, $node->changed)));
      
      $form['columnform'][$node->nid]['title'] = array(
        '#type' => 'markup',    
        '#markup' => truncate_utf8($node->title, 16, FALSE, TRUE));

      $form['columnform'][$node->nid]['ctype'] = array('#type' => 'markup', '#markup' => _cmf_get_tax_name_by_tid($node->ctype_tid));

      $form['columnform'][$node->nid]['username'] = array('#type' => 'markup', '#markup' => theme('cmf_heart_bottom_link_user', array('uid'=>$node->uid)));

      $status = array();
      $status[] = $node->status ? t('published') : t('not published');
      if ($node->promote) {
        $status[] = t('promoted');
      }
      // >0 allows for sticky-encoded weighting.
      if ($node->sticky > 0) {
        $status[] = t('sticky');
      }
  //    if ($node->moderate) {
  //      $status[] = t('moderated');
  //    }
      $form['columnform'][$node->nid]['status'] = array('#type' => 'markup', '#markup' => implode(', ', $status));

      $form['columnform'][$node->nid]['created'] = array('#type' => 'markup', '#markup' => date('Y-m-d H:i:s',$node->created));

      if (user_access('filter and manage site content') && _check_upload_address($node->type,$node->nid)) {
        $form['columnform'][$node->nid]['operations'] = array('#type' => 'markup', '#markup' => l(_cmf_heart_bottom_link_get_img('edit', t('edit')) .' '.
          t('edit'), 'node/'. $node->nid .'/edit', array('query' => $destination, 'html' => TRUE)).
          l(_cmf_heart_bottom_link_get_img('blocked', t('delete')) .' '.
          t('delete'), 'node/'. $node->nid .'/delete', array('query' => $destination, 'html' => TRUE)));
      }

      $form['columnform'][$node->nid]['weight'] =  array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => isset($node->weight) ? $node->weight : 99,
        '#delta' => 10,
        '#title_display' => 'invisible',
      );
    }
  }  
 
  if (user_access('use_cmf_heart_bottom_link_operation')) {
    $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  }

  $form['pager'] = array('#type'=>'markup', '#markup' => theme('pager')); 
  $form['columnform']['actions'] = array('#type' => 'actions');
  $form['columnform']['actions']['submit'] = array('#type' => 'submit', '#value' => t('保存排序'), '#submit' => array('cmf_heart_bottom_link_admin_nodes_form_submit'));
  return $form;
}

function cmf_heart_bottom_link_admin_nodes_sort_form_submit($form, &$form_state){
    $operations = module_invoke_all('node_operations');
    if($form_state['values']['operation']==0){
      drupal_set_message('请选择操作项!','error');
    }else{
      $operation = $operations[$form_state['values']['operation']];
      // Filter out unchecked nodes.
      $nodes = array_filter($form_state['values']['nodes']);
      if ($function = $operation['callback']) {
        // Add in callback arguments if present.
        if (isset($operation['callback arguments'])) {
          $args = array_merge(array($nodes), $operation['callback arguments']);
        }
        else {
          $args = array($nodes);
        }
        call_user_func_array($function, $args);
        
        // drupal_set_message(t('The update has been performed.'));
        $form_state['redirect'] = 'admin/content/filter/cmf_hbl';
      }
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
function cmf_heart_bottom_link_admin_nodes_sort_form_validate($form, &$form_state) {
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
function cmf_heart_bottom_link_admin_nodes_form_submit($form, &$form_state) { 
  foreach ($form_state['values']['columnform'] as $nid => $item) { 
    if(isset($item['weight'])){
      _cmf_heart_bottom_link_sort(
        'field_data_field_weight',
        array('field_weight_value' => $item['weight']),
        array('entity_id'=>$nid,'entity_type'=>'node')
      );    
    }
  }
}

/**
 * [_cmf_heart_bottom_link_getTax description]
 * @param  array  $ccarr [description]
 * @return [type]        [description]
 */
function _cmf_heart_bottom_link_getTax($ccarr=array()){
  $tax = taxonomy_get_vocabularies();
  $taxarr = array();
  $taxarr[-1] = '==请选择==';
  $lasttaxarr = array();
  foreach($ccarr as $k=>$v){
    $addarr = is_array(variable_get('cmf_tax_heart_bl_'.$k)) ? array_filter(variable_get('cmf_tax_heart_bl_'.$k)) : array();
    $lasttaxarr = $lasttaxarr + $addarr;
  }

  foreach($lasttaxarr as $k){
    $taxarr[$k] = $tax[$k]->name;
  }

  return $taxarr;
}

//根据当前用户判断该用户可以查看哪些内容类型的数据
function _cmf_heart_bottom_link_get_access_content_type(){
  $content_type = variable_get('cmf_base_heart_bl');
  //查找比较
  $accessarr = array();
  foreach (array_filter($content_type) as $type) {
    $accessarr[$type] = $type;
  }
  return $accessarr;
} 

function _cmf_heart_bottom_link_get_sort_content_type(){
  $content_type = _cmf_heart_bottom_link_get_access_content_type(); 
  foreach ($_SESSION['cmf_heart_bottom_link_overview_filter'] as $key => $value) {
    if($value[0]=='type'){
      $content_type = $value[1];
    }
  }
  return $content_type;
}

/**
 * 替换所有
 */
function _cmf_heart_bottom_link_replace_sort_all($nodes,$post){
  $postvaluearr = array();
  foreach ($nodes as $nid) {
    if(isset($post[$nid])){
      $postvaluearr[] = $post[$nid];
    }
  }
  return $postvaluearr;
}

/**
 * cmf_heart_bottom_link sort function
 */
function _cmf_heart_bottom_link_sort($tab,$value,$condition){
  $query = db_update($tab)->fields($value); 
  foreach ($condition as $key => $value) {
    if(is_array($value))
      $query->condition($key,$value,'in');
    else
      $query->condition($key,$value);
  }  
  $query->execute();
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
function cmf_heart_bottom_link_admin_nodes_export_validate($form, &$form_state) { 
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
function cmf_heart_bottom_link_admin_nodes_export_submit($form, &$form_state) {
  $nodes = array_filter($form_state['values']['nodes']);
  _cmf_heart_bottom_link_export_operation($form,$nodes);
}

/**
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results.
 */
function theme_cmf_heart_bottom_link_admin_nodes_form($form) {
  // drupal_add_css(drupal_get_path('module','cmf_heart_bottom_link').'/css/cmf_heart_bottom_link.css','top');
  drupal_add_css('#cmf_heart_bottom_link-items-table th a.active{ color:black; !important} .tabledrag-toggle-weight-wrapper a{display: none;}','inline');
  $form = $form['form'];
  $rows = array(); 
  $output = drupal_render($form['options']);
  if (isset($form['columnform']) && is_array($form['columnform'])) {     
    foreach (element_children($form['columnform']) as $key) {
      if(is_numeric($key)){
        $row = array();
        if (user_access('use_cmf_heart_bottom_link_operation')) {
          $row[] = drupal_render($form['nodes'][$key]);
        }
        if ($_SESSION['cmf_heart_bottom_link_show_nid']) {
          $row[] = drupal_render($form['columnform'][$key]['cmf_heart_bottom_link_id']);
          //添加统计信息
        }
        $form['columnform'][$key]['weight']['#attributes']['class'] = array('cmf_heart_bottom_link-item-weight');

        $row[] = drupal_render($form['columnform'][$key]['title']);
        $row[] = drupal_render($form['columnform'][$key]['ctype']);
        $row[] = drupal_render($form['columnform'][$key]['status']);
        $row[] = drupal_render($form['columnform'][$key]['username']);
        $row[] = drupal_render($form['columnform'][$key]['created']);
        if (user_access('filter and manage site content')) {
          $row[] = drupal_render($form['columnform'][$key]['operations']);
        }
        $row[] = drupal_render($form['columnform'][$key]['weight']);
        if(!empty($row)){
          $rows[] = array('data' => (array) $row, 'class'=>array('draggable'));
        }
      }
    }
  } 
  else {
    $rows[] = array(array('data' => t('Filter returned no results.'), 'colspan' => '7'));
  } 

  $table_id = 'cmf_heart_bottom_link-items-table';
  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows,'attributes' => array('id' => $table_id)));
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'cmf_heart_bottom_link-item-weight');

  if ($form['pager']['#markup']) {
    $output .= render($form['pager']);
  }
  
  $output .= drupal_render_children($form);
  return $output;
}
