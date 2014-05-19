<?php
function cmf_heart_bottom_link_settings(){
  $form = array();
  $form['cmf_heart_bottom_link_roles_seetings'] = array(
    '#type' => 'fieldset',
    '#title' => t('角色过滤设置'), 
    '#weight' => 3, 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
  );

  $form['cmf_heart_bottom_link_roles_seetings']['cmf_heart_bottom_link_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('过滤角色设置'),
    '#weight' => 5, 
    '#options' => user_roles(),
    '#default_value' => variable_get('cmf_heart_bottom_link_roles'),
  );

  $form['cmf_heart_bottom_link_sup_until'] = array(
    '#type' => 'textfield',
    '#title' => t('支持单位tid'),
    '#weight' => 1, 
    '#required' => true,
    '#default_value' => variable_get('cmf_heart_bottom_link_sup_until')
  );

  $form['cmf_heart_bottom_link_friend_link'] = array(
    '#type' => 'textfield',
    '#title' => t('友情链接tid'),
    '#weight' => 2, 
    '#required' => true,
    '#default_value' => variable_get('cmf_heart_bottom_link_friend_link')
  );

  return system_settings_form($form);
}