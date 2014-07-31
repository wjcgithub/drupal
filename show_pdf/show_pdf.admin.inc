<?php 
/**
 * [show_pdf_settings description]
 * @return [type] [description]
 */
function show_pdf_settings_form(){
  $settings = show_pdf_settings();
  $form = array();
  $form['swf_size'] = array(
	  '#type' => 'select',
	  '#title' => t('生成swf效果'),
	  '#options' => array('bitmap' => 'bitmap','storeallcharacters' => 'storeallcharacters'),
	  '#required' => true,
	  '#default_value' => $settings['swf_size']
  );

  $form['pdf2swf_bundle'] = array(
	  '#type' => 'textarea',
	  '#title' => t('pdf to swf bundle'),
	  '#required' => true,
	  '#default_value' => isset($settings['pdf2swf_bundle']) ? $settings['pdf2swf_bundle'] : ''
  );

  $form['pdf2swf_field'] = array(
	  '#type' => 'textarea',
	  '#title' => t('pdf to swf field'),
	  '#required' => true,
	  '#default_value' => isset($settings['pdf2swf_field']) ? $settings['pdf2swf_field'] : ''
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交'),
  );
  $form['#submit'] = array('show_pdf_settings_form_submit');

  return $form;
}

/**
 * [show_pdf_settings_submit description]
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function show_pdf_settings_form_submit($form, &$form_state){
  $settings = array();
  $settings['swf_size'] = $form_state['values']['swf_size'];
  $settings['pdf2swf_bundle'] = $form_state['values']['pdf2swf_bundle'];
  $settings['pdf2swf_field'] = $form_state['values']['pdf2swf_field'];

  // save
  variable_set('show_pdf_settings', serialize($settings));
  drupal_set_message(t('设置成功'));
}