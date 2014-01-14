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
function cmf_admin_nodes_form($form, $form_state) {
  global $user;
  $user_page_user = $form_state['build_info']['args'][0]; 
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
  if (user_access('administer nodes')) {
    $form['options'] = array(
       '#type' => 'fieldset', 
       '#title' => t('Update options'),
       '#prefix' => '<div class="container-inline">', 
       '#suffix' => '</div>',
    );  
    module_load_include('inc', 'node', 'node.admin');
    $options = array(); 
    foreach (module_invoke('node', 'node_operations') as $operation => $array) {
      $options[$operation] = $array['label'];
    }
    array_unshift($options, '==请选择==');
    $options['sort'] = '排序';
    unset($options['sticky']);
    unset($options['unsticky']);
    global $user;
    if(!array_key_exists(3,$user->roles)){
      unset($options['delete']);
    }
    $form['options']['operation'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => 'approve',
      );
    $form['options']['submit'] = array('#type' => 'submit', '#value' => t('更新'), '#submit' => array('cmf_admin_nodes_form_submit'));
    //$form['options']['export'] = array('#type' => 'submit', '#value' => t('导出数据'), '#submit' => array('cmf_admin_nodes_export_submit'));
  }

  // Load the nodes that we want to display.
  $form['header'] = array(
    '#type'  => 'value',
    '#value' => cmf_build_header($user_page_user),
    );

  $result = cmf_perform_query($form['header']['#value'],  NULL, $user_page_user);  
   
  // Build a table listing the appropriate nodes.
  $nodes = array();  
  while ($node = $result->fetchObject()) {
    $nodes[$node->nid] = '';

    if ($_SESSION['cmf_show_nid']) {
      $form['cmf_id'][$node->nid] = array('#type' => 'markup', '#markup' => l($node->nid, 'node/'. $node->nid, array('attributes' => array('title' => t('Node !nid', array('!nid' => $node->nid))))));
    }

    // Don't mark it if it is the current user.
    $mark = $node->uid == $user->uid ? NULL: ' '. theme('mark', array('type'=>node_mark($node->nid, $node->changed)));
    
    $form['title'][$node->nid] = array(
      '#type' => 'markup',    
      '#markup' => l(truncate_utf8(drupal_html_to_text($node->title), 16, FALSE, TRUE), 'node/'. $node->nid, array(
        'attributes' => array('title' => $node->title,
        ),
      'fragment' => 'node-'. $node->nid)) . $mark);
    $form['title_type'][$node->nid] = array('#type' => 'markup', '#markup' => _cmf_get_title_type($node->title_type));

    $form['kind'][$node->nid] = array('#type' => 'markup', '#markup' => _cmf_get_img('node', t('node')));

    $form['type'][$node->nid] = $node->type == 'forum' ? array('#type' => 'markup', '#markup' => '<p title="'.
      _cmf_get_forum($node->nid) .'">'. check_plain(node_type_get_name($node)) .'</p>') :
        array('#markup' =>  check_plain(node_type_get_name( $node))); 

    if (!_cmf_valid_user($user_page_user)) {
      $form['username'][$node->nid] = array('#type' => 'markup', '#markup' => theme('cmf_user', array('uid'=>$node->uid)));
      
    }

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
    $form['status'][$node->nid] = array('#type' => 'markup', '#markup' => implode(', ', $status));

    $form['created'][$node->nid] = array('#type' => 'markup', '#markup' => date('Y-m-d',$node->created));

    if (user_access('filter and manage site content') && _check_upload_address($node->type,$node->nid)) {
      $form['operations'][$node->nid] = array('#type' => 'markup', '#markup' => l(_cmf_get_img('edit', t('edit')) .' '.
        t('edit'), 'node/'. $node->nid .'/edit', array('query' => $destination, 'html' => TRUE)));
    }

//    if ($locale_available) {
//      $form['language'][$node->nid] = array('#type' => 'markup', '#markup' => $lang_codes[$node->language]);
//    }
    
    //统计点击率
    $form['totalcount'][$node->nid] = array('#type' => 'markup', '#markup' => $node->totalcount ? $node->totalcount : 0);
    $form['commentcount'][$node->nid] = array('#type' => 'markup', '#markup' => $node->commentcount ? $node->commentcount : 0);
    if(_check_have_type('type')){
      //首页排序
      if(!isset($node->sortnum) || !is_numeric($node->sortnum)){
        $form['sortnum'][$node->nid] = array('#type' => 'markup', '#markup' => '');
      }else{
       $num = !empty($node->sortnum) ? $node->sortnum : 20;
       if($num>20 || $num<=0) {
        $num = 20;
       }
        $form['sortnum'][$node->nid] = array(
          '#type' => 'select',
          '#default_value' => $num,
          '#options' => array(1=>'1',2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20),
          '#attributes' => array('name' => 'sort['.$node->nid.']'),
        );
      }

      //亚专科排序
      if(!isset($node->sortnum2) || !is_numeric($node->sortnum2)){
        $form['sortnum2'][$node->nid] = array('#type' => 'markup', '#markup' => '');
      }else{
         $num = !empty($node->sortnum2) ? $node->sortnum2 : 20;
         if($num>20 || $num<=0) {
          $num = 20;
         }
        $form['sortnum2'][$node->nid] = array(
          '#type' => 'select',
          '#default_value' => $num,
          '#options' => array(1=>'1',2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20),
          '#attributes' => array('name' => 'sort2['.$node->nid.']'),
        );
      }
      
      //栏目排序
      if(!isset($node->sortnum3) || !is_numeric($node->sortnum3)){
        $form['sortnum3'][$node->nid] = array('#type' => 'markup', '#markup' => '');
      }else{
        $num = !empty($node->sortnum3) ? $node->sortnum3 : 20;
         if($num>20 || $num<=0) {
          $num = 20;
         }
        $form['sortnum3'][$node->nid] = array(
          '#type' => 'select',
          '#title' => t('Weight'),
          '#default_value' => $num,
          '#options' => array(1=>'1',2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20),
          '#title_display' => 'invisible',
          '#attributes' => array('name' => 'sort3['.$node->nid.']'),
        );
      }  
      $form['sortnum'][$node->nid]['#attributes']['class'] = array('cmf-item-weight');
    }else{
      $form['sortnum'][$node->nid] = array('#type' => 'markup', '#markup' => $node->sortnum ? $node->sortnum : '');
    }
  }
 
  if (user_access('filter and manage site content')) {
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
function cmf_admin_nodes_form_validate($form, &$form_state) { 
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
function cmf_admin_nodes_form_submit($form, &$form_state) {
  $nodes = ope_nodes($form_state['values']['nodes']);
  //排序
  if($form_state['values']['operation']=='sort'){ 
    foreach ($form_state['values']['nodes'] as $nid) {
      //首页排序
      if($nid && isset($_POST['sort'][$nid])){
        // db_update('field_data_field_sort')
        //   ->fields(array(
        //     'field_sort_value' => 20
        //   )) 
        //   ->condition('field_sort_value',$_POST['sort'][$nid])
        //   ->execute();  
        db_update('field_data_field_sort')
          ->fields(array(
            'field_sort_value' => $_POST['sort'][$nid]
          )) 
          ->condition('entity_id',$nid)
          ->execute();
      }

      //亚专科排序
      if($nid && isset($_POST['sort2'][$nid])){
        // db_update('field_data_field_sort_2')
        //   ->fields(array(
        //     'field_sort_2_value' => 20
        //   )) 
        //   ->condition('field_sort_2_value',$_POST['sort2'][$nid])
        //   ->execute();  
        db_update('field_data_field_sort_2')
          ->fields(array(
            'field_sort_2_value' => $_POST['sort2'][$nid]
          )) 
          ->condition('entity_id',$nid)
          ->execute();
      }

      //栏目排序
      if($nid && isset($_POST['sort3'][$nid])){
        // db_update('field_data_field_sort_3')
        //   ->fields(array(
        //     'field_sort_3_value' => 20
        //   )) 
        //   ->condition('field_sort_3_value',$_POST['sort3'][$nid])
        //   ->execute();  
        db_update('field_data_field_sort_3')
          ->fields(array(
            'field_sort_3_value' => $_POST['sort3'][$nid]
          )) 
          ->condition('entity_id',$nid)
          ->execute();
      }
    }
  }else{
    $operations = module_invoke_all('node_operations');
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
      
      $user_page_user =  isset($form['#user_page_user']) ? $form['#user_page_user'] : NULL;
    
      if (_cmf_valid_user($user_page_user)) {
        $form_state['redirect'] = 'user/'. $user_page_user->uid .'/cmf';
      }
      else {
        $form_state['redirect'] = 'admin/content/filter';
      }
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
function cmf_admin_nodes_export_validate($form, &$form_state) { 
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
function cmf_admin_nodes_export_submit($form, &$form_state) {
  $nodes = array_filter($form_state['values']['nodes']);
  _cmf_export_operation($form,$nodes);
}

/**
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results.
 */
function theme_cmf_admin_nodes_form($form) {
  $form = $form['form'];
  $rows = array();
  $output = drupal_render($form['options']);  
  if (isset($form['title']) && is_array($form['title'])) {     
    foreach (element_children($form['title']) as $key) {
      $row = array();
      if (user_access('filter and manage site content')) {
        $row[] = drupal_render($form['nodes'][$key]);
      }
      if ($_SESSION['cmf_show_nid']) {
        $row[] = drupal_render($form['cmf_id'][$key]);
        //添加统计信息
      }
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['title_type'][$key]);
      unset($form['kind']);
      // $row[] = drupal_render($form['kind'][$key]);
      $row[] = drupal_render($form['type'][$key]);

      $user_page_user = isset($form['#user_page_user']) ? $form['#user_page_user'] : NULL;
  
      if (!_cmf_valid_user($user_page_user)) {
        $row[] = drupal_render($form['username'][$key]);
      }
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['created'][$key]);
      if ($_SESSION['cmf_content_kind']) {
        //添加统计信息
        $row[] = drupal_render($form['totalcount'][$key]);
        $row[] = drupal_render($form['commentcount'][$key]);
      }
      $row[] = drupal_render($form['sortnum'][$key]);
      $row[] = drupal_render($form['sortnum2'][$key]);
      $row[] = drupal_render($form['sortnum3'][$key]);
      if (user_access('filter and manage site content')) {
        $row[] = drupal_render($form['operations'][$key]);
      }

      if(_check_have_type('type')){
        $rows[] = $row;
      } else {
        $rows[] = array('data' => (array) $row, 'class'=>array('draggable'));
      }
    }
    //wjcPrint($form['header']['#value']);
  } 
  else {
    $rows[] = array(array('data' => t('Filter returned no results.'), 'colspan' => '7'));
  }
  
  //判断有没有排序
  if(_check_have_type('type')){
    $table_id = 'cmf-items-table';
    $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows,'attributes' => array('id' => $table_id)));
    //drupal_add_tabledrag($table_id, 'order', 'sibling', 'cmf-item-weight');
  } else {
    $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));
  }

  if ($form['pager']['#markup']) {
    $output .= render($form['pager']);
  }
  
  $output .= drupal_render_children($form);
  return $output;
}
