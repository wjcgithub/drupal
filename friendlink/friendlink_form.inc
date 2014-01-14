<?php
/**
 * Build the friendlink_admin_form form.
 *
 * @return
 *   A form array set for theming by theme_tabledrag_example_simple_form()
 */
function friendlink_admin_form($form_state) {
  $form['friendlink']['#tree'] = TRUE;
  $result = _get_friend_link();

  // Iterate through each database result.
  foreach ($result as $item) {
    $form['friendlink'][$item->nid] = array(
      'name' => array(
        '#markup' => check_plain($item->title),
      ),

      'link' => array(
        '#markup' => check_plain($item->url),
      ),

      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $item->weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
    );
    if (user_access('administrater_friendlink') && user_access('edit own friendlink content') && user_access('delete own friendlink content')) {
        $destination = drupal_get_destination();
        $form['friendlink'][$item->nid]['operations'][] = array(
          '#type' => 'markup', 
          '#markup' => l(_pdf2img_get_img('edit',t('编辑')).' '.
        t('编辑'), 'node/'. $item->nid .'/edit', array('query' => $destination, 'html' => TRUE))
        );
        $form['friendlink'][$item->nid]['operations'][] = array(
        '#type' => 'markup', 
        '#markup' => l(_pdf2img_get_img('delete',t('删除')).' '.
        t('删除'), 'node/'. $item->nid .'/delete', array('query' => $destination, 'html' => TRUE))
        );
      }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('保存'));
  return $form;
}

/**
 * save order by
 */
function friendlink_admin_form_submit($form, &$form_state){
  foreach ($form_state['values']['friendlink'] as $id => $item) {
    db_query(
      "UPDATE {field_data_field_friendlink_weight} SET field_friendlink_weight_value = :weight WHERE entity_id = :id",
      array(':weight' => $item['weight'], ':id' => $id)
    );
  }
}

/**
 * Theme callback for the friendlink_admin_form form.
 *
 * @return
 *   The rendered tabledrag form
 */
function theme_friendlink_admin_form($variables) {
  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  // Iterate over each element in our $form['friendlink'] array.
  foreach (element_children($form['friendlink']) as $id) {

  $form['friendlink'][$id]['weight']['#attributes']['class'] = array('friendlink-weight');

  $rows[] = array(
      'data' => array(
        // Add our 'name' column.
        drupal_render($form['friendlink'][$id]['name']),
        // Add our 'link' column.
        drupal_render($form['friendlink'][$id]['link']),
        // Add our 'weight' column.
        drupal_render($form['friendlink'][$id]['weight']), 
        drupal_render($form['friendlink'][$id]['operations']), 
      ),

      'class' => array('draggable'),
    );
  }

  $header = array(t('链接名称'), t('链接地址'), t('排序'), t('操作'));

  $table_id = 'friendlink-table';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'friendlink-weight');
  $a = '<a href="/node/add/friendlink?destination=admin/friendlink">添加友情链接</a>';
  return $a.$output;
}
  
/**
 * Theme callback for the friendlink.
 *
 * @return
 *   The rendered friendlink
 */  
function theme_friendlink_render($variables){
  $form = $variables['link'];
  $link = ''; 
  foreach ($form as $item) {
    $link .=l($item->title,$item->url,array('attributes'=>array('target'=>'_blank')));
  }
  return $link;
} 