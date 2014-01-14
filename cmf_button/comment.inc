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
function comment_operations($action = NULL) {
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
function cmf_admin_comments_form($form_state, $user_page_user = NULL) {

  $destination = drupal_get_destination();
  
  // Build an 'Update options' form.
  if (user_access('filter and manage site content')) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update options'),
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
      );
    $options = array();
    foreach (comment_operations() as $key => $value) {
      $options[$key] = $value[0];
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
      '#value' => cmf_build_header($user_page_user),
    ); 

  $result = cmf_perform_query($form['header']['#value'], NULL, $user_page_user);  

  $comments = array();
  // Build a table listing the appropriate comments.
  while ($comment = $result->fetchObject()) {  
    $comments[$comment->cid] = '';

    if ($_SESSION['cmf_show_nid']) {
      $form['cmf_id'][$comment->cid] = array('#type' => 'markup', '#markup' => l($comment->nid, 'node/'. $comment->nid, array(
        'attributes' => array('title' => t('Node !nid, Comment !cid', array('!nid' => $comment->nid, '!cid' => $comment->cid))),
        'fragment' => 'comment-'. $comment->cid)));
    }

    $form['title'][$comment->cid] = array('#type' => 'markup', '#markup' => l($comment->subject, 'node/'. $comment->nid,
      array('attributes' => array('title' => check_plain($comment->comment)),
      'fragment' => 'comment-'. $comment->cid)));

    $form['kind'][$comment->cid] = array('#type' => 'markup', '#markup' => _cmf_get_img('comment', t('comment')));

    $form['type'][$comment->cid] = $comment->type == 'forum' ? array('#type' => 'markup', '#markup' => '<p title="'.
      _cmf_get_forum($comment->nid) .'">'. theme('cmf_type', array('type' => $comment->type)) .'</p>') :
      array('#type' => 'markup', '#markup' =>  theme('cmf_type', array('type' => $comment->type)));

    if (!_cmf_valid_user($user_page_user)) {
      $form['username'][$comment->cid] = array('#type' => 'markup', '#markup' => theme('cmf_user', array($comment->uid)));
    }

    $form['status'][$comment->cid] =  array('#type' => 'markup', '#markup' =>  (!$comment->status ? t('not published') : t('published')));

    $form['created'][$comment->cid] = array('#type' => 'markup', '#markup' => format_date($comment->created, 'small'));

    if (user_access('filter and manage site content')) {
      $form['operations'][$comment->cid] = array('#type' => 'markup', '#markup' => l(_cmf_get_img('edit', t('edit')) .' '.
        t('edit'), 'comment/'.$comment->cid.'/edit/', array('query' => $destination, 'html' => TRUE)));
    }
  }

  if (user_access('filter and manage site content')) {
    $form['comments'] = array('#type' => 'checkboxes', '#options' => $comments);
  }
  $form['pager'] = array('#type'=>'markup', '#markup' => theme('pager'));
  $form['#user_page_user'] = $user_page_user;
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
function cmf_admin_comments_form_validate($form, &$form_state) {
  $form_state['values']['comments'] = array_diff($form_state['values']['comments'], array(0));
  if (count($form_state['values']['comments']) == 0) {
    form_set_error('', t('No items selected.'));
    drupal_goto('admin/content/filter');
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
function cmf_admin_comments_form_submit($form, &$form_state) { 
  $operations = comment_operations(); 
  if ($operations[$form_state['values']['operation']][1]) {
    // Extract the appropriate database query operation.
    $query = $operations[$form_state['values']['operation']][1]; 
    foreach ($form_state['values']['comments'] as $cid => $value) {
      if ($value) {
        // Perform the update action, then refresh node statistics.
        db_query($query, array($cid));
        $comment = comment_load($cid);
        _comment_update_node_statistics($comment->nid);
        // Allow modules to respond to the updating of a comment.
        comment_invoke_comment($comment, $form_state['values']['operation']);
      }
    }
    cache_clear_all();
    drupal_set_message(t('The update has been performed.'));

    $user_page_user = $form['#user_page_user'];
    if (_cmf_valid_user($user_page_user)) {
      $form_state['redirect'] = 'user/'. $user_page_user->uid .'/cmf';
    }
    else {
      $form_state['redirect'] = 'admin/content/filter';
    }
  }
}

/**
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results
 */
function theme_cmf_admin_comments_form($form) {
  $form = $form['form'];
  $rows = array();
  $output = drupal_render($form['options']);
  if (isset($form['title']) && is_array($form['title'])) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      if (user_access('filter and manage site content')) {
        $row[] = drupal_render($form['comments'][$key]);
      }
      if ($_SESSION['cmf_show_nid']) {
        $row[] = drupal_render($form['cmf_id'][$key]);
      }
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['kind'][$key]);
      $row[] = drupal_render($form['type'][$key]);
      
      $user_page_user = $form['#user_page_user'];
      
      if (!_cmf_valid_user($user_page_user)) {
        $row[] = drupal_render($form['username'][$key]);
      }
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['created'][$key]);
      if (user_access('filter and manage site content')) {
        $row[] = drupal_render($form['operations'][$key]);
      }
      $rows[] = $row; 
    } 
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
function comment_invoke_comment(&$comment, $op) {
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