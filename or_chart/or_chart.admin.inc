<?php
/**
 * Administration page for baidu map
 */
function or_chart_settings() {
  $form = array();
  $form['or_chart_info'] = array(
    '#type' => 'item',
    '#markup' => '<div id="scroll-to-top-prev-container">Create a new taxonomy to <a target="_blank" href="'.base_path().'admin/structure/taxonomy/add">Create a taxonomy</a> and get this taxonomy vid fill in the below vid field</div>',
  );
  $form['or_chart_vid'] = array(
    '#type' => 'textfield',
    '#title' => t('vid for url'),
    '#default_value' => variable_get('or_chart_vid', ''),
  );
  $form['or_chart_phpexcel_path'] = array(
    '#type' => 'textfield',
    '#title' => t('or chart phpexcel path'),
    '#default_value' => variable_get('or_chart_phpexcel_path', ''),
  );
  return system_settings_form($form);
}