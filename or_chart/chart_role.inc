<?php
/**
 * 
 */
function _adminStatRole(){
  module_load_include('inc', 'or_chart','chart_role_admin');
  $output = '';
  //add search condition
  $cff = drupal_get_form('_or_chart_filter_form'); 
  $output = drupal_render($cff); 
  //add search content 
  $canf = drupal_get_form('_or_chart_admin_nodes_form');  
  $output .= drupal_render($canf);
   drupal_add_js(drupal_get_path('module', 'or_chart').'/wjc.page.js'); 
  return $output.'<input type="hidden" id="basepath" value="'.base_path().'">';
}

/**
 * List node administration filters that can be applied.
 *
 * @param $user 
 *  (object) if a user profile page then user object (defaults to false).
 *
 * @return array with filter properties.
 *   Any property starting with '#' goes directly into the filter form.
 *   '#options' is also used in function _or_chart_filter_form_submit.
 *   'title' is the text that will show on the filter form.
 *   'single_use' (bool) determines if the filter will be disabled if it is already in use.
 *   'whole_value'  "before" if the value is complete and shown before "is",
 *      "after" if the value is complete and shown after "is",
 *      "no" if it is a portion ("contains"), such as a substring.
 *   'disable' is a list of other filters to remove if this one is selected.
 *   'where' sets a simple WHERE clause for the query (with substitution).
 *   'join' sets a simple JOIN clause for the query (with substitution).
 *   'query_build' provides a funtion for more complex query clauses.
 *   If validation and/or submission handling is needed, use the #element_validate and #submit elements.
 *   Submit handlers must set the return value in $form_state['values'] array.
 *
 *  NOTE: for the 'where' clause some node fields are translated to comment-equivalent
 *   field names if appropriate. See function _or_chart_perform_query for 'comment' and 'both.'
 */
function _or_chart_filters() {
  // Make the array static so it is built only once per page load.
  static $filters;
  if (isset($filters)) {
    return $filters;
  }
  
  $filters = array();
   
  $filters['created_after'] = array(
    'title' => t('该时间点之前'),
    'single_use' => TRUE,
    'whole_value' => 'after',
    'where' => "n.created >= ?",
    'tags' => 'd',     
    '#type' => 'date',
    '#element_validate' => array('date_validate', '_or_chart_date_validate'),
    '#submit' => array('_or_chart_date_handler'),
    '#default_value' => array('year' => date('Y'), 'month' => 1, 'day' => 1),
    '#prefix' => '<div class="date-inline">',
    '#suffix' => '</div>',
    );

  $filters['created_before'] = array(
    'title' => t('该时间点之后'),
    'single_use' => TRUE,
    'whole_value' => 'after',
    'where' => "n.created <= ?",
    'tags' => 'd',    
    '#type' => 'date',
    '#element_validate' => array('date_validate', '_or_chart_date_validate'),
    '#submit' => array('_or_chart_date_handler'),
    '#default_value' => array('year' => date('Y'), 'month' => 12, 'day' => 31),
    '#prefix' => '<div class="date-inline">',
    '#suffix' => '</div>',
    );

    $filters['users'] = array(
      'title' => t('user name'),
      'single_use' => TRUE,
      'whole_value' => 'before',
      'disable' => array('user', 'role', 'blocked'),
      'where' => "u.name = ?",
      'tags' => 's',    
      '#type' => 'textfield',
      '#submit' => array('_or_chart_users_handler'),
      '#autocomplete_path' => 'admin/statistics/userauto',
      );

    $filters['role'] = array(
      'title' => t('user role'),
      'single_use' => TRUE,
      'whole_value' => 'before',
      'where' => "u.uid = ur.uid AND ur.rid = ?",
      'tags' => 'd',    
      'join' => "INNER JOIN {users_roles} ur ON u.uid = ur.uid",
      '#type' => 'select',
      '#options' => _or_chart_get_roles('names'),
      );
  //drupal_alter('_or_chart_filters', $filters);
  return $filters;
}

/**
 * FILTERS
 */
/**
 * Defines the form for content administration filters.
 *
 * @ingroup forms
 *
 * @param (optional) true if the filter to be built serves the user profile page
 * @return array with forms properties
 */
function _or_chart_filter_form($form, &$form_state) {   
  //调用提示信息  
  $session = &$_SESSION['_or_chart_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = _or_chart_filters();
   drupal_add_css(drupal_get_path('module', 'or_chart') .'/or_chart.css');
  $i = 0;
  
  //add notice  info
  foreach ($session as $filter) { 
    list($type, $value) = $filter;
    // Special handling by type of filter.
    switch ($type) {
      case 'created_after':
      case 'created_before':
        // Format as human date.
        $fmt = variable_get('date_format_short', variable_get('date_format_short_custom', 'Y M j')); 
        $fmt = trim(str_replace(array('H', 'i', ':', '-'), '', $fmt)); 
        $value = format_date($value, 'short', $fmt); 
        break;

      default:
        if (isset($filters[$type]['#options'])) {
          $value = $filters[$type]['#options'][$value];
        }
    }

    // Add applicable filter verbiage. Note '@' and '%' do check_plain.
    $trans_strings = array(
      '%filter_name' => $filters[$type]['title'],
      '%filter_value' => $value,
      );
    if ($i++) {
      $and = '<br><em>'. t('and') .'</em> ';
    }
    else {
      $and = NULL;
    }
    
    //after  before 是指的is 是在前还是在后
    switch ($filters[$type]['whole_value']) {
      case 'after':
        $form['filters']['current'][] = array('#type'=>'markup', '#markup' => $and . t('is %filter_name %filter_value', $trans_strings));
        break;

      case 'before':
        $form['filters']['current'][] = array('#type'=>'markup', '#markup' => $and . t('%filter_name is %filter_value', $trans_strings));
        break;

      case 'no':
        $form['filters']['current'][] = array('#type'=>'markup', '#markup' => $and . t('%filter_name contains %filter_value', $trans_strings));
        break;
    }

    // Remove mutually exclusive filters.
    if (isset($filters[$type]['disable'])) {
      foreach ($filters[$type]['disable'] as $exclude) {
        unset($filters[$exclude]);
      }
    }

    //如果已经被查询了，那么就删除
    if ($filters[$type]['single_use']) {
      unset($filters[$type]);
    }
  } 
  // Prepare form fields for filters.
  foreach ($filters as $key => $filter) {
     if($filter['#type']!='hidden'){  
        $names[$key] = $filter['title'];
     }  
        // Normal form field handling.
     $form['filters']['status'][$key] = array(); 
     foreach ($filter as $element => $value) { 
          // If the filter element name begins with '#' then it is for the form.
          if (drupal_substr($element, 0, 1) == '#') {     
            $form['filters']['status'][$key][$element] = $value;
          }
      }  
  }
    
  // Building radio buttons.
  $keyz = array_keys($names); 
  $form['filters']['filter'] = array(
    '#type'          => 'radios',
    '#options'       => $names,
    '#default_value' => $keyz[0],
    '#attributes' => array('class' => array('multiselect')),
    );
   
  // Building buttons depending on the filter state.
  $form['filters']['buttons']['submit'] = array(
    '#type'  => 'submit',
    '#value' => (count($session) ? t('细化') : t('过滤')),
    '#submit' => array('_or_chart_filter_form_refine'),
    );

  if (count($session)) {
    $form['filters']['buttons']['undo'] = array(
      '#type' => 'submit',
      '#submit' => array('_or_chart_filter_form_undo'),
      '#value' => t('撤销'),
    );
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#submit' => array('_or_chart_filter_form_reset'),
      '#value' => t('重置'),
    );
  }  
  return $form;
}

/**
 * Submit handler for 'Undo' button.
 */
function _or_chart_filter_form_undo($form, &$form_state) {
  array_pop($_SESSION['_or_chart_overview_filter']);
}

/**
 * Submit handler for 'Reset' button.
 */
function _or_chart_filter_form_reset($form, &$form_state) {
  $_SESSION['_or_chart_overview_filter'] = array();
}

/**
 * Submit handler for 'Apply' button.
 */
function _or_chart_filter_form_apply($form, &$form_state) {
    $form_state['redirect'] = 'admin/statistics/role';
}


/**
 * Handle post-validation form submission.
 *
 * @ingroup forms
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */
function _or_chart_filter_form_refine($form, &$form_state) {
  $filters = _or_chart_filters();

  if (isset($form_state['values']['filter'])) {
    $filter = $form_state['values']['filter'];

    // Convert AJAX search value to select box value.

    // If a filter has specified a submit function, call it. (Only allow one value.)
    if (isset($filters[$filter]['#submit'])) {
      $form_state['values'][$filter] = call_user_func($filters[$filter]['#submit'][0], $form, $form_state);
    }
    // Repeating this allows submit handlers (e.g. users) to alter the filter name.
    $filter = $form_state['values']['filter'];

    // Flatten the options array to accommodate hierarchical/nested options.
    // If there are no options, fake it.
    if (isset($filters[$filter]['#options'])) {
      $flat_options = form_options_flatten($filters[$filter]['#options']); 
    }
    else {
      $flat_options = array($form_state['values'][$filter] => 1);
    }
    // If the option is selected or there are no options, set the session value.
    if (isset($flat_options[$form_state['values'][$filter]])) {
      //如果为过滤条件那么选取隐藏值做为查询值
      if($filter=='category'){
        $_SESSION['_or_chart_overview_filter'][] = array($filter, $form_state['values']['categoryhide']);
      }else{
        $_SESSION['_or_chart_overview_filter'][] = array($filter, $form_state['values'][$filter]);
      }
    } 
  }
}