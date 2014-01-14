<?php
/**
 * Administration page for baidu map
 */
function advertising_settings() {
  $form = array();
  $options = array();
  foreach (advertising_get_types() as $type => $info) {
    $options[$type] = $info->label;
  }
  $form['advertising_content_type'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#required' => true,
    '#title' => t('广告位类型'),
    '#default_value' => variable_get('advertising_content_type', ''),
  );
  return system_settings_form($form);
}