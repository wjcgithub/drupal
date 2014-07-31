<?php

/**
 * This field handler aggregates operations that can be done on a advertising
 * under a single field providing a more flexible way to present them in a view
 */
class vaf_operation extends views_handler_field {
  function construct() {
    parent::construct();

	// $pros = new RefTools('views_handler_field', true);
    // dpm($pros->Cinfo());
    $this->additional_fields['nid'] = 'nid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['entity_settings']['entity_select'] = array(
      'default' => array(),
    );
    $options['entity_settings']['field_select'] = array(
      'default' => array(),
    );

    return $options;
  }

  function options_form(&$form, &$form_state) { 
	parent::options_form($form, $form_state);
	// $default_sheng = !empty($form_state['values']['sheng']) ? $form_state['values']['sheng'] : '';
	$form['entity_settings'] = array( 
		'#tree' => true,
	 	'#type' => 'fieldset', 
	 	'#title' => t('select a entity'), 
	 	'#collapsible' => TRUE, 
	 	'#collapsed' => TRUE, 
	 	'#weight' => 99
	);

	$modules = module_invoke_all('entity_info');
	$modulearr = array();
	foreach ($modules as $key => $value) {
		$modulearr[$key] = $key; 
	}
	$form['entity_settings']['entity_select'] = array( 
	  	'#title' => t('entity type'), 
	  	'#options' => $modulearr,   
	  	'#type' => 'select', 
	  	'#description' => t('Choose a entity type'), 
      	'#default_value' => isset($this->options['entity_settings']['entity_select']) ? $this->options['entity_settings']['entity_select'] : array(),
	);

	$form['entity_settings']['field_select'] = array( 
	  	'#title' => t('node field'), 
	  	'#options' => _get_all_entity_field(),   
	  	'#type' => 'select', 
	  	'#description' => t('Choose a node field'), 
      	'#default_value' => isset($this->options['entity_settings']['field_select']) ? $this->options['entity_settings']['field_select'] : array(),
	);
  }
  
  function render($values) {
    $nid = $this->get_value($values, 'nid');
    $links = array();
    $useentity = $this->options['entity_settings']['entity_select'];
    $usefield = $this->options['entity_settings']['field_select'];
    list($bundle,$field) = explode('<>', $usefield);

    $links['vaf-auditing']=menu_get_item('admin/vaf/auditing/'.$useentity.'/'.$bundle.'/'.$field.'/'.$nid.'/ajax/approve');
    $links['vaf-auditing']['attributes'] = array('class' => array_merge(cdialog_link_class(), array('vaf-'.$nid.'-manage-operations')));
    drupal_alter('vaf_operatioin_links',$links,$this->view,$values,$this->options['entity_settings']['field_select']);
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations' ,'vaf-'.$nid.'-auditing'))));
    }
  }
}
