<?php
function adv_comments_settings(){
  $form = array();
  $form['cmf_seetings'] = array(
    '#type' => 'fieldset',
    '#title' => t('高级评论管理'), 
    '#weight' => 1, 
    '#collapsible' => TRUE, 
    '#collapsed' => true,
  );
  $form['cmf_seetings']['adv_comments_cc_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('选择要被过滤的内容类型'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('adv_comments_cc_settings'),
  );
  return system_settings_form($form);
}