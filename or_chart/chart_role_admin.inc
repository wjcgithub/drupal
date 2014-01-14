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
 * _or_chart
 * @return array with forms properties
 */
function _or_chart_admin_nodes_form($form, $form_state) {     
  global $user;
  $destination = drupal_get_destination();   
  $lang_codes = array('und' => t('Neutral'));

  // Build an 'Update options' form.
  if (user_access('manage and filter site statistics')) {
    $form['options'] = array(
       '#type' => 'fieldset', 
       '#title' => t('可执行操作'),
       '#prefix' => '<div class="container-inline">', 
       '#suffix' => '</div>',
    );    
    $form['options']['export'] = array('#type' => 'submit', '#value' => t('导出数据'), '#submit' => array('_or_chart_admin_nodes_export_submit'));
  }

  // Load the nodes that we want to display.
  $form['header'] = array(
    '#type'  => 'value',
    '#value' => _or_chart_build_header(),
    );

  $result = _or_chart_perform_query($form['header']['#value'],  NULL);  
   
  // Build a table listing the appropriate nodes.
  $nodes = array();  
  while ($node = $result->fetchObject()) {
    $nodes[$node->nid] = '';

   $form['cmf_id'][$node->nid] = array('#type' => 'markup', '#markup' => l($node->nid, 'node/'. $node->nid, array('attributes' => array('title' => t('Node !nid', array('!nid' => $node->nid))))));

    // Don't mark it if it is the current user.
    $mark = $node->uid == $user->uid ? NULL: ' '. theme('mark', array('type'=>node_mark($node->nid, $node->changed)));
    
    $form['title'][$node->nid] = array(
      '#type' => 'markup',  
      '#markup' => l(drupal_substr($node->title,0,20), 'node/'. $node->nid, array(
        'attributes' => array('title' => check_plain($node->body),
        ),
      'fragment' => 'node-'. $node->nid)) . $mark);

    $form['username'][$node->nid] = array('#type' => 'markup', '#markup' => theme('_or_chart_user', array('uid'=>$node->uid)));

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

    if (user_access('statistics_role')) {
      $form['operations'][$node->nid][] = array('#type' => 'markup', '#markup' => l(_or_chart_get_img('edit', t('编辑')) .
        t('编辑').'<br>', 'node/'. $node->nid .'/edit', array('query' => $destination, 'html' => TRUE)));
      // $form['operations'][$node->nid][] = array('#type' => 'markup', '#markup' => l(_or_chart_get_img('count', t('统计')) .
      //   t('统计'), 'node/'. $node->nid .'/edit', array('query' => $destination, 'html' => TRUE)));
    }
    
    //统计点击率
    $form['totalcount'][$node->nid] = array('#type' => 'markup', '#markup' => $node->totalcount);
    $form['commentcount'][$node->nid] = array('#type' => 'markup', '#markup' => $node->commentcount);
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
function _or_chart_admin_nodes_form_validate($form, &$form_state) { 
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
function _or_chart_admin_nodes_export_validate($form, &$form_state) { 
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
function _or_chart_admin_nodes_export_submit($form, &$form_state) {
  $nodes = array_filter($form_state['values']['nodes']);
  _or_chart_export_operation($form,$nodes);
}

/**
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results.
 */
function theme__or_chart_admin_nodes_form($form) {    //wjcPrint($form['form']);
  $form = $form['form'];
  $rows = array();
  $output = drupal_render($form['options']);  
  if (isset($form['title']) && is_array($form['title'])) {     // wjcPrint(element_children($form['title']) );
    foreach (element_children($form['title']) as $key) {
      $row = array();
      if (user_access('statistics_role')) {
        $row[] = drupal_render($form['nodes'][$key]);
      }
      $row[] = drupal_render($form['cmf_id'][$key]);
      //添加统计信息
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['created'][$key]);

      //添加统计信息
      $row[] = drupal_render($form['totalcount'][$key]);
      $row[] = drupal_render($form['commentcount'][$key]);
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
