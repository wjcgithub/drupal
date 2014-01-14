<?php
function cmf_settings(){
  $form = array();
  $form['cmf_seetings'] = array(
    '#type' => 'fieldset',
    '#title' => t('高级过滤管理'), 
    '#weight' => 1, 
    '#collapsible' => TRUE, 
    '#collapsed' => true,
  );
  $form['cmf_seetings']['cmf_cc_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('选择要被过滤的内容类型'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('cmf_cc_settings'),
  );
  
  $form['cmf_seetings']['cmf_one_type_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('哪些角色的用户可以使用一级栏目'),
    '#options' => cmf_get_roles('names'),
    '#default_value' => variable_get('cmf_one_type_settings'),
  );
  
  return system_settings_form($form);
}