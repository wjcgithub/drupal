<?php
/**
 * Administration page for baidu map
 */
function advertising_settings() {
  drupal_add_css(drupal_get_path('module','adv_block').'/css/adv_select_class.css');
  drupal_add_js(drupal_get_path('module','adv_block').'/js/adv_block.js');
  $form = array();
  $options = array();
  foreach (advertising_get_types() as $type => $info) {
    $options[$type] = $info->label;
  }

  $form['adv_basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('基本设置'),
    '#weight' => 1, 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
  );

  $form['adv_info_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('内容设置'),
    '#weight' => 2, 
    '#collapsible' => TRUE, 
    '#collapsed' => false,
  );

  $form['adv_basic_settings']['advertising_content_type'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#required' => true,
    '#title' => t('广告位类型'),
    '#default_value' => variable_get('advertising_content_type', ''),
  );

  $form['adv_basic_settings']['advertising_tax'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#element_validate' => array('element_validate_integer'),
    '#title' => t('广告位位置术语id'),
    '#default_value' => variable_get('advertising_tax', ''),
  );

  $form['adv_info_settings']['advertising_front_top'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#element_validate' => array('element_validate_integer'),
    '#title' => t('首页头部'),
    '#default_value' => variable_get('advertising_front_top', ''),
  );

  $form['adv_info_settings']['advertising_front_center'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#element_validate' => array('element_validate_integer'),
    '#title' => t('首页中部'),
    '#default_value' => variable_get('advertising_front_center', ''),
  );

  $form['adv_info_settings']['advertising_front_right'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#element_validate' => array('element_validate_integer'),
    '#title' => t('首页中部'),
    '#default_value' => variable_get('advertising_front_right', ''),
  );

  $form['adv_info_settings']['advertising_news_right'] = array(
    '#type' => 'textfield',
    '#required' => true,
    '#element_validate' => array('element_validate_integer'),
    '#title' => t('新闻列表页右侧'),
    '#default_value' => variable_get('advertising_news_right', ''),
  );

  $form['adv_select'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="adv_select_class" id="adv_select_id">'.get_note_select().'</div>',
    '#title' => t('提供信息处'),
  );

  drupal_alter('advertising_settings',$form);
  
  return system_settings_form($form);
}

function get_note_select(){
  $resarr = _get_adv_list();
  if(!empty($resarr)){
    foreach ($resarr as $key => $value) {
      $items = array();
      $advid = $value->advertising_id;
      $advinfo = entity_load('advertising',array($advid));
      $uri = $advinfo[$advid]->field_adv_img['und'][0]['uri'];
      $alt = $advinfo[$advid]->field_adv_img['und'][0]['alt'];
      $title = $advinfo[$advid]->field_adv_img['und'][0]['title'];
      $link = $advinfo[$advid]->field_adv_link['und'][0]['safe_value'];
      $name = $advinfo[$advid]->field_adv_name['und'][0]['safe_value'];
      $imgstr = theme('image_style',  
        array('style_name' => '105x77',
          'path' => $uri,
          'alt' => $alt,
          'title' =>$title,
        )
      );
      $items[] = $advid;
      $items[] = filter_xss($name);
      $items[] = $imgstr;
      $op[$key] = $items;
    }
    return theme('table', array('header' => array('advid' => t('advid'), 'name' => t('广告位名称'), 'imgstr' => t('广告图片')), 'rows' => $op));
  }
}