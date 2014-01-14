<?php
/**
 * @file
 * advertising_type.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function advertising_type_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'advertising-advertising_heart-field_adv_img'
  $field_instances['advertising-advertising_heart-field_adv_img'] = array(
    'bundle' => 'advertising_heart',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'advertising',
    'exclude_cv' => 0,
    'field_name' => 'field_adv_img',
    'label' => '广告位图片',
    'required' => 1,
    'settings' => array(
      'alt_field' => 1,
      'default_image' => 0,
      'exclude_cv' => FALSE,
      'file_directory' => 'advertising/img/[current-date:common_date_format]',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'filefield_sources' => array(
          'filefield_sources' => array(
            'attach' => 0,
            'clipboard' => 0,
            'plupload' => 0,
            'reference' => 0,
            'remote' => 0,
            'upload' => 'upload',
          ),
          'source_attach' => array(
            'absolute' => 0,
            'attach_mode' => 'move',
            'path' => 'file_attach',
          ),
          'source_imce' => array(
            'imce_mode' => 0,
          ),
          'source_reference' => array(
            'autocomplete' => 0,
          ),
        ),
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'advertising-advertising_heart-field_adv_img_style'
  $field_instances['advertising-advertising_heart-field_adv_img_style'] = array(
    'bundle' => 'advertising_heart',
    'default_value' => NULL,
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'advertising',
    'exclude_cv' => 0,
    'field_name' => 'field_adv_img_style',
    'label' => '广告位图片样式',
    'required' => 1,
    'settings' => array(
      'exclude_cv' => FALSE,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'advertising-advertising_heart-field_adv_link'
  $field_instances['advertising-advertising_heart-field_adv_link'] = array(
    'bundle' => 'advertising_heart',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'advertising',
    'exclude_cv' => 0,
    'field_name' => 'field_adv_link',
    'label' => '广告位图片链接地址',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          '_p_br_' => '_p_br_',
          'editer_view' => 'editer_view',
          'filtered_html' => 'filtered_html',
          'front_editer' => 'front_editer',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            '_p_br_' => array(
              'weight' => 0,
            ),
            'editer_view' => array(
              'weight' => 0,
            ),
            'filtered_html' => array(
              'weight' => 0,
            ),
            'front_editer' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
          ),
        ),
      ),
      'exclude_cv' => FALSE,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'advertising-advertising_heart-field_adv_name'
  $field_instances['advertising-advertising_heart-field_adv_name'] = array(
    'bundle' => 'advertising_heart',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'advertising',
    'exclude_cv' => 0,
    'field_name' => 'field_adv_name',
    'label' => '广告位名称',
    'required' => 1,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          '_p_br_' => '_p_br_',
          'editer_view' => 'editer_view',
          'filtered_html' => 'filtered_html',
          'front_editer' => 'front_editer',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            '_p_br_' => array(
              'weight' => 0,
            ),
            'editer_view' => array(
              'weight' => 0,
            ),
            'filtered_html' => array(
              'weight' => 0,
            ),
            'front_editer' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
          ),
        ),
      ),
      'exclude_cv' => FALSE,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'advertising-advertising_heart-field_adv_url'
  $field_instances['advertising-advertising_heart-field_adv_url'] = array(
    'bundle' => 'advertising_heart',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '多个路径使用回车符分隔',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'advertising',
    'exclude_cv' => 0,
    'field_name' => 'field_adv_url',
    'label' => '广告位图片出现路径',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          '_p_br_' => '_p_br_',
          'editer_view' => 'editer_view',
          'filtered_html' => 'filtered_html',
          'front_editer' => 'front_editer',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            '_p_br_' => array(
              'weight' => 0,
            ),
            'editer_view' => array(
              'weight' => 0,
            ),
            'filtered_html' => array(
              'weight' => 0,
            ),
            'front_editer' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
          ),
        ),
      ),
      'exclude_cv' => FALSE,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('多个路径使用回车符分隔');
  t('广告位名称');
  t('广告位图片');
  t('广告位图片出现路径');
  t('广告位图片样式');
  t('广告位图片链接地址');

  return $field_instances;
}
