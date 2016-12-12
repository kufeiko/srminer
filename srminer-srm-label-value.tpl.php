<?php

//dpm($srm_data);

?>

<h2>You profile in SRM</h2>

<div class="srm-data-intro blockquote blockquote-closed">
  Buccaneer run a shot across bow coxswain for hearties. Tack Plate Fleet pirate run a shot across the bow crack Jennys tea cup hulk jolly boat boom poop deck sutler. List barkadeer mizzenmast carouser me pink take a caulk interloper belaying pin yard. Code of conduct provost hornswaggle furl strike colors run a shot across the bow rope's end lookout handsomely sutler.
</div>

<div class="srm-data">

  <?php foreach ($srm_data as $delta => $item): ?>


    <?php if ($item['label'] && $item['value']): ?>

      <div class="srm-data-item row  <?php print $delta % 2 ? 'odd' : 'even'; ?>">
        <div class="srm-label col-xs-12 col-sm-6 col-lg-3">
          <?php print $item['label'] ?>
        </div>
        <div class="srm-value col-xs-12 col-sm-6 col-lg-9">
          <?php print $item['value'] ?>
        </div>
      </div>
    <?php endif; ?>

  <?php endforeach; ?>


</div>
