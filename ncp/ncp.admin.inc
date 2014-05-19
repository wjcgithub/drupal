<?php
function ncp_admin_settings(){
  $form = array();
  $form['ncp_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('内容分页设置'),
    '#weight' => 1, 
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('ncp_type'), 
  );

  return system_settings_form($form);
}