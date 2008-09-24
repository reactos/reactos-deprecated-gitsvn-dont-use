<?php
session_start();

$test = new CaptchaSecurityImages();

class CaptchaSecurityImages {

	function generateCode($characters) {
		/* list all possible characters, similar looking characters and vowels have been removed */
		/*$possible = '23456789bcdfghjkmnpqrstvwxyz';
		$code = '';
		$i = 0;
		while ($i < $characters) { 
			$code .= substr($possible, mt_rand(0, strlen($possible)-1), 1);
			$i++;
		}
		return $code;*/
		$s = "";
		for ($n=0; $n<10; $n++) {
			$s .= chr(rand(0, 255));
		}
		$s = base64_encode($s);   // base64-set, but filter out unwanted chars
		$s = preg_replace("/[+\/=IG0ODQRtl]/i", "", $s);  // strips hard to discern letters, depends on used font type
		//$s = substr($s, 0, $characters);
		$s = substr($s, 0, rand(4, $characters));
		return $s;
	}
	
	function CaptchaSecurityImages() {
		global $font_data;		
		//echo "<p>!".$font_data[2][0]."</p>";
	
		$width = 240;
		$height = 80;
	
		$cur_random = mt_rand(0, count($font_data));
		
		$font = $font_data[$cur_random][0];
		$font_size_mult = $font_data[$cur_random][1];
		$characters = $font_data[$cur_random][2];
		
		//echo "<p>FONT[".$cur_random."]: ".$font."</p>";
	
		$font = "logon/captcha/".$font;
	
		$code = $this->generateCode($characters);
		/* font size will be 75% of the image height */
		$font_size = $height * $font_size_mult;
		$image = @imagecreate($width, $height) or die('Cannot initialize new GD image stream');
		/* set the colours */
		$background_color = imagecolorallocate($image, 255, 255, 255);
		$text_color = imagecolorallocate($image, 20, 40, 100);
		$noise_color = imagecolorallocate($image, 100, 120, 180);
		/* generate random dots in background */
		for( $i=0; $i<($width*$height)/3; $i++ ) {
			imagefilledellipse($image, mt_rand(0,$width), mt_rand(0,$height), 1, 1, $noise_color);
		}
		/* generate random lines in background */
		for( $i=0; $i<($width*$height)/150; $i++ ) {
			imageline($image, mt_rand(0,$width), mt_rand(0,$height), mt_rand(0,$width), mt_rand(0,$height), $noise_color);
		}
		/* create textbox and add text */
		$textbox = imagettfbbox($font_size, 0, $font, $code) or die('Error in imagettftext function (1)');//$test2 = new CaptchaSecurityImages();

		$x = ($width - $textbox[4])/2;
		$y = ($height - $textbox[5])/2;
		imagettftext($image, $font_size, 0, $x, $y, $text_color, $font , $code) or die('Error in imagettftext function (2)');
		
		/*for ($x=0; $x<250; $x+=27) {
			imageline($image, $x, 0, $x, 70, 0x6478B4);
			imageline($image, 0, $x, 250, $x, 0x6478B4);
		}*/
		
		$num = rand(10,25);
		$x = $width;
		$y = $height;
		$s = rand(0,270);
		for ($n=0; $n<$num; $n++) {
			imagesetthickness($image, rand(1,2));
			imagearc($image,
				rand(0.1*$x, 0.9*$x), rand(0.1*$y, 0.9*$y),  //x,y
				rand(0.1*$x, 0.3*$x), rand(0.1*$y, 0.3*$y),  //w,h
				$s, rand($s+5, $s+90),     // s,e
				rand(0,1) ? 0xFFFFFF : 0x000000   // col
			);
		}
		imagesetthickness($image, 1);
	
				
		/* output captcha image to browser */
		header('Content-Type: image/jpeg');
		imagejpeg($image);
		imagedestroy($image);
		$_SESSION['rdf_security_code'] = $code;
	}
}

?>