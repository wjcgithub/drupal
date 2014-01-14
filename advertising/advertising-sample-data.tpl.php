<?php

/**
 * @file
 * Example tpl file for theming a single advertising-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $advertising: The advertising object this status is derived from
 */
?>

<div class="advertising-status">
  <?php print '<strong>advertising Sample Data:</strong> ' . $advertising_sample_data = ($advertising_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>