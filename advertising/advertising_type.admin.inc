<?php

/**
 * @file
 * Advertising type editing UI.
 */

/**
 * UI controller.
 */
class AdvertisingTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage advertising entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the advertising type editing form.
 */
function advertising_type_form($form, &$form_state, $advertising_type, $op = 'edit') {

  if ($op == 'clone') {
    $advertising_type->label .= ' (cloned)';
    $advertising_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $advertising_type->label,
    '#description' => t('The human-readable name of this advertising type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($advertising_type->type) ? $advertising_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $advertising_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'advertising_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this advertising type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save advertising type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$advertising_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete advertising type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('advertising_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function advertising_type_form_submit(&$form, &$form_state) {
  $advertising_type = entity_ui_form_submit_build_entity($form, $form_state);
  $advertising_type->save();
  $form_state['redirect'] = 'admin/structure/advertising_types';
}

/**
 * Form API submit callback for the delete button.
 */
function advertising_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/advertising_types/manage/' . $form_state['advertising_type']->type . '/delete';
}
