<?php

/**
 * Comment operations. We offer different update operations depending on
 * which comment administration page we're on.
 *
 * @param $action
 *   The comment administration page.
 * @return
 *   An associative array containing the offered operations.
 */
function adv_comment_operations($action = NULL) {
  if ($action == 'publish') {
    $operations = array(
      'publish' => array(t('Publish the selected comments'), 'UPDATE {comment} SET status = '. COMMENT_PUBLISHED .' WHERE cid = ?'),
      'delete' => array(t('Delete the selected comments'), ''),
    );
  }
  else if ($action == 'unpublish') {
    $operations = array(
      'unpublish' => array(t('Unpublish the selected comments'), 'UPDATE {comment} SET status = '. COMMENT_NOT_PUBLISHED .' WHERE cid = ?'),
      'delete' => array(t('Delete the selected comments'), ''),
    );
  }
  else {
    $operations = array(
      'publish' => array(t('Publish the selected comments'), 'UPDATE {comment} SET status = '. COMMENT_PUBLISHED .' WHERE cid = ?'),
      'unpublish' => array(t('Unpublish the selected comments'), 'UPDATE {comment} SET status = '. COMMENT_NOT_PUBLISHED .' WHERE cid = ?'),
      'delete' => array(t('Delete the selected comments'), ''),
    );
  }
  return $operations;
}

/**
 * @file
 * @brief Content management filter comment operations file.
 *
 * This file contains all the comment functions used by the module.
 */

/**
 * Defines the form for comments administration filter results.
 *
 * @ingroup forms
 *
 * @return array with forms properties
 */
function acf_admin_comments_form($form, $form_state) {
  $destination = drupal_get_destination();
  
  // Build an 'Update options' form.
  if (user_access('use_administer_comments')) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update options'),
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
      );
    $options = array();
    foreach (adv_comment_operations() as $key => $value) {
      $options[$key] = $value[0];
      if($GLOBALS['user']->uid!=1){
        unset($options['delete']);
      }
    }
    $form['options']['operation'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => 'publish',
      );
    $form['options']['submit'] = array('#type' => 'submit', '#value' => t('Update'));
  }

    // Load the comments that we want to display.
    $form['header'] = array(
      '#type'  => 'value',
      '#value' => acf_build_header(),
    ); 

  $result = acf_perform_query($form['header']['#value']);  

  $comments = array();
  // Build a table listing the appropriate comments.
  while ($comment = $result->fetchObject()) {
    $comments[$comment->cid] = '';
    $form['title'][$comment->cid] = array('#type' => 'markup', '#markup' => l(check_plain(truncate_utf8(drupal_html_to_text($comment->subject), 24, FALSE, TRUE)), 'node/'. $comment->nid,
      array('attributes' => array('title' => check_plain($comment->comment)),
      'fragment' => 'comment-'. $comment->cid)));

    $form['type'][$comment->cid] = $comment->type == 'forum' ? array('#type' => 'markup', '#markup' => '<p title="'.
      _cmf_get_forum($comment->nid) .'">'. theme('cmf_type', array('type' => $comment->type)) .'</p>') :
      array('#type' => 'markup', '#markup' =>  theme('cmf_type', array('type' => $comment->type)));

    $ntitle = _get_node_title($comment->nid);
    $form['posted_in'][$comment->cid] = array('#type' => 'markup', '#markup' => l(check_plain(truncate_utf8(drupal_html_to_text($ntitle), 24, FALSE, TRUE)), 'node/'. $comment->nid,
      array('attributes' => array('title' => check_plain($ntitle)))));  
        
    $form['username'][$comment->cid] = array('#type' => 'markup', '#markup' => theme('acf_user', array('uid'=>$comment->uid)));

    $form['good_comment'][$comment->cid] = array('#type' => 'markup', '#markup' => theme('acf_good_comments', array('cid'=>$comment->cid)));

    $form['status'][$comment->cid] =  array('#type' => 'markup', '#markup' =>  (!$comment->status ? t('not published') : t('published')));

    $form['created'][$comment->cid] = array('#type' => 'markup', '#markup' => date('Y-m-d H:i:s',$comment->created));

    if (user_access('ope_administer_comments')) {
      $form['operations'][$comment->cid] = array('#type' => 'markup', '#markup' => l(_cmf_get_img('edit', t('edit')) .' '.
        t('edit'), 'comment/'.$comment->cid.'/edit/', array('query' => $destination, 'html' => TRUE)));
    }
  }

  if (user_access('use_administer_comments')) {
    $form['comments'] = array('#type' => 'checkboxes', '#options' => $comments);
  }
  $form['pager'] = array('#type'=>'markup', '#markup' => theme('pager'));
  return $form;
}

/**
 * Form validation before submit. \n
 * We can't execute any 'Update options' if no comments were selected.
 *
 * @ingroup forms
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */
function acf_admin_comments_form_validate($form, &$form_state) {
  $form_state['values']['comments'] = array_diff($form_state['values']['comments'], array(0));
  if (count($form_state['values']['comments']) == 0) {
    form_set_error('', t('No items selected.'));
    drupal_goto('admin/comments/filter');
  }
}

/**
 * Handle post-validation form submission.
 * Execute the chosen 'Update option' on the selected comments, such as
 * publishing, unpublishing or deleting.
 *
 * @ingroup forms
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */
function acf_admin_comments_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/comments/filter';
  $operations = adv_comment_operations(); 
  if (isset($operations[$form_state['values']['operation']][1])) {
    // Extract the appropriate database query operation.
    $query = $operations[$form_state['values']['operation']][1]; 
    foreach ($form_state['values']['comments'] as $cid => $value) {
      if ($value) {
        // Perform the update action, then refresh node statistics.
        $comment = comment_load($cid);
        db_query($query, array($cid));
        _comment_update_node_statistics($comment->nid);
        if($form_state['values']['operation']=='unpublish' && $comment->status==1){
          $txnid = db_query('select txn_id from {userpoints_txn} where uid=:uid and entity_id=:entity_id and entity_type=:entity_type and tid=:tid', array(':uid'=>$comment->uid,':entity_id'=>$comment->cid,':entity_type'=>'comment',':tid'=>695))->fetchField();
          if(empty($txnid)){
            _unpublish_comment($comment->uid,$comment->cid,$comment->subject);
          }
        }
        // Allow modules to respond to the updating of a comment.
        adv_comment_invoke_comment($comment, $form_state['values']['operation']);
      }
    }
    $form_state['redirect'] = 'admin/comments/filter';
    // cache_clear_all();
  }else{
    // _comment_admin_overview_submit($form, $form_state);
  }
}

/**
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results
 */
function theme_acf_admin_comments_form($form) {
  drupal_add_css('#acf-admin-comments-form th a.active{ color:black; !important}','inline');
  $form = $form['form'];
  $rows = array();
  $output = drupal_render($form['options']);
  if (isset($form['title']) && is_array($form['title'])) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      $row[] = drupal_render($form['comments'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['posted_in'][$key]);
      $row[] = drupal_render($form['type'][$key]);
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['created'][$key]);
      $row[] = drupal_render($form['good_comment'][$key]);
      if (user_access('ope_administer_comments')) {
        $row[] = drupal_render($form['operations'][$key]);
      }
      $rows[] = $row; 
    } 
  }
  else {
    $rows[] = array(array('data' => t('Filter returned no results.'), 'colspan' => (count($row)-2) ));
  }

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));
  if ($form['pager']['#markup']) {
    $output .= render($form['pager']);
  }

  $output .= drupal_render_children($form);

  return $output;
}



/**
 * =========================================wjc add from drupal6=======================================================
 */

/**
 * Invoke a hook_comment() operation in all modules.
 *
 * @param &$comment
 *   A comment object.
 * @param $op
 *   A string containing the name of the comment operation.
 * @return
 *   The returned value of the invoked hooks.
 */
function adv_comment_invoke_comment(&$comment, $op) {
  $return = array();
  foreach (module_implements('comment') as $name) {
    $function = $name .'_comment';
    $result = $function($comment, $op);
    if (isset($result) && is_array($result)) {
      $return = array_merge($return, $result);
    }
    else if (isset($result)) {
      $return[] = $result;
    }
  }
  return $return;
}

/**
 * =========================================wjc add from drupal6=======================================================
 */