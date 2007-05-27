<?php

	$roscms_intern_usrgrp_policy_view_basic = false;
	$roscms_intern_usrgrp_policy_view_advanced = false;
	$roscms_intern_usrgrp_policy_translate = false;
	$roscms_intern_usrgrp_policy_edit_basic = false;
	$roscms_intern_usrgrp_policy_edit_advanced = false;


	// view_basic
	if ($roscms_intern_usrgrp_trans == true || 
		$roscms_intern_usrgrp_transm == true || 
		$roscms_intern_usrgrp_men == true || 
		$roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_view_basic = true;
	}

	// view_advanced
	if ($roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_view_advanced = true;
	}

	// translate
	if ($roscms_intern_usrgrp_trans == true || 
		$roscms_intern_usrgrp_transm == true || 
		$roscms_intern_usrgrp_men == true || 
		$roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_translate = true;
	}

	// edit_basic
	if ($roscms_intern_usrgrp_men == true || 
		$roscms_intern_usrgrp_team == true || 
		$roscms_intern_usrgrp_dev == true || 
		$roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_edit_basic = true;
	}

	// edit_advanced
	if ($roscms_intern_usrgrp_admin == true || 
		$roscms_intern_usrgrp_sadmin == true)
	{
		$roscms_intern_usrgrp_policy_edit_advanced = true;
	}



?>