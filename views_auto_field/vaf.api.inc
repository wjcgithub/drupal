<?php
/**
 * [HOOK_vaf_operatioin_links_alter description]
 * @param [type] $links  [description]
 * @param [type] $view   [description]
 * @param [type] $values [description]
 * @see  vaf_operation.inc  render function
 */
function HOOK_vaf_operatioin_links_alter(&$links,$view,$values,$field){

}

/**
 * [HOOK_vaf_pre_confirm description]
 * @param [type] $entity_type [description]
 * @param [type] $nid         [description]
 */
function HOOK_vaf_pre_confirm_alter($entity_type, $nid){
  $wrapper = entity_metadata_wrapper($entity_type, $nid);
  $status = $wrapper->field_name->value(); 
  if(isset($status) && $status>0){
    drupal_set_message(t('对不起，该内容已经被审核！'),'warning');
    drupal_goto('custom_url');
  }
}

/**
 * [HOOK_vaf_confirm_form_show_alter description]
 * @param [type] $form       [description]
 * @param [type] $form_state [description]
 */
function HOOK_vaf_confirm_form_show_alter(&$form, $form_state){
	$form['reason'] = array(
	    '#type' => 'textarea',
	    '#rows' => 5,
	    '#title' => t('不通过的原因:'),
	    '#size' => 5,
	    '#states' => array(
	      'visible' => array(
	        '#edit-status-2' => array('value' => 2),
	      ),
	   ),
	);
}

/**
 * [HOOK_vaf_auditing_value_alter description]
 * @param [type] $entitynodeobj [description]
 * @param [type] $form_state    [description]
 */
function HOOK_vaf_auditing_value_alter(&$entitynodeobj,$form_state){
    //添加积分
    $txnid = _check_isnot_add_points($nodeuid,$nodenid,$nodetype);
    if(empty($txnid)){
        $point = variable_get('user_'.$nodetype.'_points');
        $params = array (
          'uid' => $nodeuid,
          'description' => t('desc'),
          'display' => false,
          'moderate' => NULL,
          'operation' => t('Insert'),
          'tid' => 'tid',
          'points' => 'point',
          'entity_id' => 'nodenid',
          'entity_type' => 'node',
        );
        $res = userpoints_userpointsapi($params);
        if($res['status']){
          //收信人      title    desc     发信人
          privatemsg_new_thread(array(user_load($nodeuid)), '系统消息', 'desc', array('author'=>user_load(1)));
        }else{
          drupal_set_message('发送积分失败');
        }
    }
}

