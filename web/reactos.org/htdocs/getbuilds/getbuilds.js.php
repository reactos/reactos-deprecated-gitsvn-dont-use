<?php
/*
  PROJECT:    ReactOS Website
  LICENSE:    GPL v2 or any later version
  FILE:       web/reactos.org/htdocs/getbuilds/getbuilds.js.php
  PURPOSE:    Easily download prebuilt ReactOS Revisions
  COPYRIGHT:  Copyright 2007 Colin Finck <mail@colinfinck.de>
*/
?>

function loadingsplash(zeroone)
{
	if (zeroone == 1)
		document.getElementById("ajaxloadinginfo").style.visibility = "visible";
	else
		document.getElementById("ajaxloadinginfo").style.visibility = "hidden";
}

function ajaxGet(action, parameters, data)
{
	loadingsplash(1);

	var http_request = false;

	switch(action)
	{
		case "getfiles":
			var url = "ajax-getfiles.php?" + parameters;
			var callback = "getfilesCallback(http_request, data);";
			break;
	}
	
	if (window.XMLHttpRequest)
	{
		// Mozilla, Safari, ...
		http_request = new XMLHttpRequest();
	}
	else if (window.ActiveXObject)
	{
		// IE
		try
		{
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		}
		catch (e)
		{
			try
			{
				http_request = new ActiveXObject("Microsoft.XMLHTTP");
			}
			catch (e) {}
		}
	}

	if (!http_request)
	{
		alert("Giving up :( Cannot create an XMLHTTP instance");
		return false;
	}
	
	http_request.open("GET", url, true);
	http_request.setRequestHeader("If-Modified-Since", "Sat, 1 Jan 2000 00:00:00 GMT");			// Bypass the IE Cache
	http_request.send(null);
	
	http_request.onreadystatechange = function()
	{
		if(http_request.readyState == 4)
		{
			if(http_request.status == 200)
				eval(callback);
			else
				alert("AJAX Request problem!" + "\n\nError Code: " + http_request.status + "\nError Text: " + http_request.statusText + "\nURL: " + url);
		}
	}
}

function getfilesCallback(http_request, revs)
{
	if( http_request.responseXML == null )
	{
		alert( http_request.responseText );
		loadingsplash(0);
		return;
	}
	
	// Prepare the table
	var datatable = '<table class="datatable" cellspacing="0" cellpadding="1">';
	datatable += '<thead><tr class="head"><th class="fname"><?php echo $getbuilds_langres["filename"]; ?></th><th class="fsize"><?php echo $getbuilds_langres["filesize"]; ?></th><th class="fdate"><?php echo $getbuilds_langres["filedate"]; ?></th></tr></thead>';
	datatable += '<tbody>';
	
	var files = http_request.responseXML.getElementsByTagName("file");

	if( files.length == 0 )
		datatable += '<tr class="odd"><td><?php echo $getbuilds_langres["nofiles1"]; ?>' + revs + '<?php echo $getbuilds_langres["nofiles2"]; ?></td><td>&nbsp;</td><td>&nbsp;</td></tr>';
	else
	{
		var oddeven = false;
		
		for( var i = 0; i < files.length; i++ )
		{
			var fname = files[i].getElementsByTagName("name")[0].firstChild.data;
			var fsize = files[i].getElementsByTagName("size")[0].firstChild.data;
			var fdate = files[i].getElementsByTagName("date")[0].firstChild.data;
			var flink = '<a href="<?php echo $ISO_DOWNLOAD_URL; ?>' + fname + '">';
			oddeven = !oddeven;
			
			datatable += '<tr class="' + (oddeven ? "odd" : "even") + '" onmouseover="tr_mouseover(this);" onmouseout="tr_mouseout(this);">';
			datatable += '<td>' + flink + '<img src="images/cd.png" alt=""> ' + fname + '</a></td>';
			datatable += '<td>' + flink + fsize + '</a></td>';
			datatable += '<td>' + flink + fdate + '</a></td>';
			datatable += '</tr>';
		}
	}
	
	datatable += '</tbody></table>';
	
	document.getElementById("filetable").innerHTML = datatable;
	loadingsplash(0);
}

function setrowcolor(elem, color)
{
	tdl = elem.getElementsByTagName("td");
	
	for( var i = 0; i < tdl.length; i++ )
		tdl[i].style.background = color;
}

function tr_mouseover(elem)
{
	setrowcolor( elem, "#FFFFCC" );
}

function tr_mouseout(elem)
{
	if( elem.className == "odd" )
		setrowcolor( elem, "#DDDDDD" );
	else
		setrowcolor( elem, "#EEEEEE" );
}

var from;
var to;

function getrevnums()
{
	var rev = document.getElementById("revnum").value;
	
	if( isNaN(rev) || rev < 1 )
	{
		// Maybe the user entered a revision range
		var hyphen = rev.indexOf("-");
		
		if( hyphen > 0 )
		{
			from = rev.substr( 0, hyphen );
			to = rev.substr( hyphen + 1 );
		}
		
		if( hyphen <= 0 || isNaN(from) || isNaN(to) )
		{
			alert("Invalid revision number!");
			return false;
		}
	}
	else
	{
		from = rev;
		to = rev;
	}
	
	return true;
}

function prevrev()
{
	if( getrevnums() )
	{
		from--;
		
		// 25700 is the lowest rev on the server at the time, when this script has been written
		// There is no harm if this rev does not exist anymore on the FTP server, it's just a min value
		if(from < 25700)
			return;
		
		document.getElementById("revnum").value = from;
	}
}

function nextrev()
{
	if( getrevnums() )
	{
		from++;
		document.getElementById("revnum").value = from;
	}
}

function showrev()
{
	if( getrevnums() )
		ajaxGet( 'getfiles', 'from=' + from + '&to=' + to, document.getElementById("revnum").value );
}

function checkrevnum(elem)
{
	elem.value = elem.value.replace( /[^[0-9-]/g, "");
}
