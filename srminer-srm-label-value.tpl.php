<?php

//dpm($srm_data);

?>

<h2>You profile in SRM</h2>

<div class="srm-data-intro">
  <p class="lead">Here you can review the information about you in our SRM.</p>
</div>

<div class="srm-data">
  <?php foreach ($srm_data as $delta => $item): ?>
    <?php if ($item['label'] && $item['value']): ?>
      <div class="srm-data-item row  <?php print $delta % 2 ? 'odd' : 'even'; ?>">
        <div class="srm-label col-xs-12 col-sm-6 col-lg-3">
          <strong><?php print $item['label'] ?></strong>
        </div>
        <div class="srm-value col-xs-12 col-sm-6 col-lg-9">
          <?php print $item['value'] ?>
        </div>
      </div>
    <?php endif; ?>
  <?php endforeach; ?>
</div>
