<?php
/**
 * [_vaf_info_auditing description]
 * @param  [type] $nid [description]
 * @param  [type] $js  [description]
 * @return [type]      [description]
 */
function _vaf_info_auditing($entity_type, $bundle, $field, $nid, $js = NULL){
  if (!$js) {
    return drupal_get_form('_vaf_info_auditing_form');
  }
  //confirm is not continue
  drupal_alter('vaf_pre_confirm', $entity_type, $nid);
  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => t('审核'),
    'ajax' => TRUE,
    'entity_info' => array('entity_type'=>$entity_type, 'bundle'=>$bundle, 'field'=>$field, 'nid'=>$nid)
  );
  $output = ctools_modal_form_wrapper('_vaf_info_auditing_form', $form_state);
  if (!empty($form_state['executed'])) {
    if($form_state['executestatus']){
      $output = array();
      $output[] = ctools_modal_command_dismiss();
      $output[] = array(
        'command' => 'vaf_success',
        'data' => array('nid' => $nid, 'auditingmsg'=>$form_state['auditingmsg']),
      );
      
    }else{
      $output = array();
      $output[] = ctools_modal_command_dismiss();
      $output[] = array(
        'command' => 'vaf_error',
        'data' => array('nid' => $nid, 'auditingmsg'=>$form_state['auditingmsg']),
      );
    }
  }
  echo ajax_render($output); 
  exit;
}

/**
 * [_vaf_info_auditing_form description]
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function _vaf_info_auditing_form($form,$form_state){
  $form = array();
  $form['entity_info'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['entity_info']
  );
  $selectvalue = _vaf_get_auditing_value($form_state['entity_info']['entity_type'], $form_state['entity_info']['bundle'], $form_state['entity_info']['field']);

  if(!empty($selectvalue)){
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('审核状态'),
      '#required' => true,
      '#default_value' => array(),
      '#options' => $selectvalue
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('确定')
  );
  $form['#validate'] = array('vaf_auditing_form_validate');
  $form['#submit'] = array('vaf_auditing_form_submit');
  drupal_alter('vaf_confirm_form_show', $form, $form_state);
  return $form;
}

/**
 * Validate handler for he_signup_approve_form
 *
 * @see he_signup_approve_form
 */
function vaf_auditing_form_validate($form, &$form_state) { 
  $status = isset($form_state['values']['status']) ? intval($form_state['values']['status']) : '';
  if (empty($status)) {
    form_set_error('status', t('请至少选中一个选项！'));
  }
}

/**
 * [vaf_auditing_form_submit description]
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function vaf_auditing_form_submit($form, &$form_state) {
  $status = $form_state['values']['status'];
  $entity_type = $form_state['values']['entity_info']['entity_type'];
  $nid = $form_state['values']['entity_info']['nid']; 
  $entitynodeobj = entity_metadata_wrapper($entity_type,$nid); 
  $entitynodeobj->$form_state['values']['entity_info']['field']->set($status);
  //add alter
  drupal_alter('vaf_auditing_value',$entitynodeobj,$form_state);
    if($entitynodeobj->save()){
      $form_state['executestatus'] = true;
    }else{
      $form_state['executestatus'] = false;
    }
}