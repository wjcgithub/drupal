<?php

/**
 * @file
 * Advertising editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class AdvertisingUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;
    $items[$this->path] = array(
      'title' => 'Advertisings',
      'description' => 'Add edit and update advertisings.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of advertisings.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a advertising',
      'description' => 'Add a new advertising',
      'page callback'  => 'advertising_add_page',
      'access callback'  => 'advertising_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'advertising.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (advertising_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'advertising_form_wrapper',
        'page arguments' => array(advertising_create(array('type' => $type->type))),
        'access callback' => 'advertising_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'advertising.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing advertising entities
    $items[$this->path . '/advertising/' . $wildcard] = array(
      'page callback' => 'advertising_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'advertising_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'advertising.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/advertising/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/advertising/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'advertising_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'advertising_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'advertising.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing advertisings
    $items['advertising/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'advertising_page_title',
      'title arguments' => array(1),
      'page callback' => 'advertising_page_view',
      'page arguments' => array(1),
      'access callback' => 'advertising_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Advertising Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('advertising_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a advertising.
 *
 * @param $advertising
 *   The advertising object being edited by this form.
 *
 * @see advertising_edit_form()
 */
function advertising_form_wrapper($advertising) {
  // Add the breadcrumb for the form's location.
  advertising_set_breadcrumb();
  return drupal_get_form('advertising_edit_form', $advertising);
}


/**
 * Form callback wrapper: delete a advertising.
 *
 * @param $advertising
 *   The advertising object being edited by this form.
 *
 * @see advertising_edit_form()
 */
function advertising_delete_form_wrapper($advertising) {
  // Add the breadcrumb for the form's location.
  //advertising_set_breadcrumb();
  return drupal_get_form('advertising_delete_form', $advertising);
}


/**
 * Form callback: create or edit a advertising.
 *
 * @param $advertising
 *   The advertising object to edit or for a create form an empty advertising object
 *     with only a advertising type defined.
 */
function advertising_edit_form($form, &$form_state, $advertising) {
  // Add the field related form elements.
  $form_state['advertising'] = $advertising;
  field_attach_form('advertising', $advertising, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save advertising'),
    '#submit' => $submit + array('advertising_edit_form_submit'),
  );
  
  if (!empty($advertising->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete advertising'),
      '#suffix' => l(t('Cancel'), 'admin/content/advertisings'),
      '#submit' => $submit + array('advertising_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'advertising_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the advertising form
 */
function advertising_edit_form_validate(&$form, &$form_state) {
  $advertising = $form_state['advertising'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('advertising', $advertising, $form, $form_state);
}


/**
 * Form API submit callback for the advertising form.
 * 
 * @todo remove hard-coded link
 */
function advertising_edit_form_submit(&$form, &$form_state) {
  $advertising = entity_ui_controller('advertising')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the advertising and go back to the list of advertisings
  
  // Add in created and changed times.
  if ($advertising->is_new = isset($advertising->is_new) ? $advertising->is_new : 0){
    $advertising->created = time();
  }

  $advertising->changed = time();
  
  $advertising->save();
  $form_state['redirect'] = 'admin/content/advertisings';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function advertising_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/advertisings/advertising/' . $form_state['advertising']->advertising_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a advertising.
 *
 * @param $advertising
 *   The advertising to delete
 *
 * @see confirm_form()
 */
function advertising_delete_form($form, &$form_state, $advertising) {
  $form_state['advertising'] = $advertising;

  $form['#submit'][] = 'advertising_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete advertising %name?', array('%name' => $advertising->name)),
    'admin/content/advertisings/advertising',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for advertising_delete_form
 */
function advertising_delete_form_submit($form, &$form_state) {
  $advertising = $form_state['advertising'];

  advertising_delete($advertising);

  drupal_set_message(t('The advertising %name has been deleted.', array('%name' => $advertising->name)));
  watchdog('advertising', 'Deleted advertising %name.', array('%name' => $advertising->name));

  $form_state['redirect'] = 'admin/content/advertisings';
}



/**
 * Page to add Advertising Entities.
 *
 * @todo Pass this through a proper theme function
 */
function advertising_add_page() {
  $controller = entity_ui_controller('advertising');
  return $controller->addPage();
}


/**
 * Displays the list of available advertising types for advertising creation.
 *
 * @ingroup themeable
 */
function theme_advertising_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="advertising-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer advertising types')) {
      $output = '<p>' . t('Advertising Entities cannot be added because you have not created any advertising types yet. Go to the <a href="@create-advertising-type">advertising type creation page</a> to add a new advertising type.', array('@create-advertising-type' => url('admin/structure/advertising_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No advertising types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative advertising pages.
 */
function advertising_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Advertisings'), 'admin/content/advertisings'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



