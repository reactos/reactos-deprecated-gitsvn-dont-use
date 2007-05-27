<?php

	echo "<h1>Pages - RosCMS v2 - AJAX driven</h1>";


?>
<script type="text/javascript" language="javascript">
<!--

	var nres=1;


	function selcb(cbid) {
		if (cbid.substr(0,2) == 'cb') { /* check for checkbox id */
			cbid = cbid.substr(2);
		}

		if (document.getElementById("cb"+cbid).checked == true) {
			document.getElementById("cb"+cbid).checked = false;
		}
		else {
			document.getElementById("cb"+cbid).checked = true;
		} 
	}

	function setrowcolor(rownum,rowcolor) {
		if (!document.getElementById || !document.getElementsByTagName || !document.getElementById("tr"+rownum)) {
			return;
		}
		
		cell_arr = document.getElementById("tr"+rownum).getElementsByTagName('td');
		
		for (i=0; i<cell_arr.length; i++) {
			cell_arr[i].style.backgroundColor = rowcolor;
		}
	}
	
	markedrows = new Array(); //global
	function hlrow(rownum,hlmode) {
		switch(hlmode) {
			case 1: //on mouseover
				setrowcolor(rownum,"#ffffcc");
				break;
			case 2: //on mouseout
				if (markedrows["tr"+rownum]!=1) {
					if (rownum%2) setrowcolor(rownum,'#dddddd');
					else setrowcolor(rownum,'#eeeeee');			
				}
				else {
					setrowcolor(rownum,"#ffcc99");	
				}
				break;
			case 3: //on click
				if (markedrows["tr"+rownum]!=1) {
					setrowcolor(rownum,"#ffcc99");
					markedrows["tr"+rownum]=1;
					//rcol_menu();
				}
				else {
					if (rownum%2) setrowcolor(rownum,"#dddddd");
					else setrowcolor(rownum,"#eeeeee");
					markedrows["tr"+rownum]=0;
				}
				break;
		}
	}
	
	function select_all_checkboxes(zeroone) {
		var scb = true;
		
		if (zeroone == 1) {
			scb = true;
		}
		else {
			scb = false;
		}
	
		for (var i=1; i<=nres; i++) {
			document.getElementById("cb"+i).checked = scb;
		}
	}

	function select_all(zeroone) {
		for (var i=1; i<=nres; i++) {
			markedrows["tr"+i] = zeroone;
			if (zeroone==1) {
				setrowcolor(i,"#ffcc99");
			}
			else {
				if (i%2) setrowcolor(i,"#dddddd");
				else setrowcolor(i,"#eeeeee");
			}
		}
		select_all_checkboxes(zeroone);
	}
		
	function add_js_extras() {
		if (!document.getElementById) return;
		
		/*cell_arr = document.getElementById("tr1").getElementsByTagName('td');
		if (cell_arr.length > 2) { c1 = 0; c2 = 1; showCM=true;}
		else { c1 = 0; c2 = 1; showCM=false;}*/
		c1 = 0; c2 = 1;
		
		//context menu
		//if (showCM) set_cmbuttons();
		
		for (i=1; i<=nres; i++) {
			//row highlighting
			if (hlRows) {
				document.getElementById("tr"+i).onmouseover = function() {hlrow(this.id.substr(2,4),1);}
				document.getElementById("tr"+i).onmouseout = function() {hlrow(this.id.substr(2,4),2);}
				document.getElementById("tr"+i).getElementsByTagName('td')[c1].onclick = function() {hlrow(this.parentNode.id.substr(2,4),3); selcb(this.parentNode.id.substr(2,4));}
				//document.getElementById("tr"+i).getElementsByTagName('td')[c1].onclick = function() {hlrow(this.parentNode.id.substr(2,4),3); alert(this.parentNode.id.substr(2,4));}
				//document.getElementById("tr"+i).getElementsByTagName('td')[c2].onclick = function() {hlrow(this.parentNode.id.substr(2,4),3);}
			}
			
			//retranslate on dblclick
			if (retrDC) {
//				document.getElementById("tr"+i).getElementsByTagName('td')[c1].ondblclick = function() {location.href="/?s="+c1Arr[this.parentNode.id.substr(2,4)];}
				//document.getElementById("tr"+i).getElementsByTagName('td')[c2].ondblclick = function() {location.href="/?s="+c2Arr[this.parentNode.id.substr(2,4)];}
			}
		}
	}


	function do_something() {
		var i=0;
		var n_ids=0; //number of ids in cookie str
		//var ids2add=new Array();
		var mvstr = " ";
		
		var currentcolor = "";
		
		//alert(document.getElementById("tr"+1).getElementsByTagName('td')[1].style.backgroundColor);
		
		//alert("loop:"+nres);
		for (var i=1; i<=nres; i++) {
			//alert('tr'+i+', id: '+document.getElementById('tr'+i).getElementsByTagName('td')[1].id);
			//alert(document.getElementById("tr"+i).getElementsByTagName('td')[1].style.backgroundColor);
			currentcolor = document.getElementById("tr"+i).getElementsByTagName('td')[1].style.backgroundColor;
			if(currentcolor == "rgb(255, 204, 153)" || currentcolor == "#ffcc99") {
				if (mvstr.indexOf("-"+ i +"-")==-1) {
					n_ids++;
					if (n_ids>500) {
						alert("Maximum: 500 entries");
						break;
					}
					//alert("tr"+i);
					mvstr = mvstr +"-"+ i +"-";
				}
			}
		}
		mvstr = mvstr.replace(/--/g, "-");
		alert("selected: "+ n_ids + ", "+ mvstr);
	}
	
	function select_stars(zeroone) {
		select_all(0); /* deselect all */
		
		var sstar = 'cStarOff';
		
		if (zeroone == 1) {
			sstar = 'cStarOn';
		}
		else {
			sstar = 'cStarOff';
		}

		for (var i=1; i<=nres; i++) {
			if (document.getElementById("tr"+i).getElementsByTagName('td')[1].getElementsByTagName('div')[0].className == sstar) {
				setrowcolor(i,"#ffcc99");
				document.getElementById("cb"+i).checked = true;
				markedrows["tr"+i]=1;
			}
			else {
				document.getElementById("cb"+i).checked = false;
				markedrows["tr"+i]=0;
			}
		}
	}
	
	function select_nds(ndsb) {
		select_all(0); /* deselect all */
		
		var sstar1 = 'odd';
		var sstar2 = 'even';
		
		switch (ndsb) {
			default:
			case 'stable':
				sstar1 = 'odd';
				sstar2 = 'even';
				break;
			case 'new':
				sstar1 = 'new';
				sstar2 = 'new';
				break;
			case 'draft':
				sstar1 = 'new';
				sstar2 = 'new';
				break;
			/* @todo: 'archive' */
		}
		
		for (var i=1; i<=nres; i++) {
			if (document.getElementById("tr"+i).className == sstar1 || document.getElementById("tr"+i).className == sstar2) {
				setrowcolor(i,"#ffcc99");
				document.getElementById("cb"+i).checked = true;
				markedrows["tr"+i]=1;
			}
			else {
				document.getElementById("cb"+i).checked = false;
				markedrows["tr"+i]=0;
			}
		}
	}

	
-->
</script>

<style type="text/css">
<!--

	.datatable {
		/*border:1px dashed red;*/
		overflow: 				hidden;
		min-width: 350px;
		width: 100%;
	}
	
	.datatable tr.head th,
	.datatable tr.head {
		background-image:   none;
		background:         #5984C3;
		color:				#FFFFFF;
		font-family: 		Verdana, Arial, Helvetica, sans-serif;
		/*font-size:			80%;*/
		font-style:			normal;
		text-align:         left;
	}
	
	.datatable tr.odd td,
	.datatable tr.even td {
		color:				#000000;
		font-family: 		Verdana, Arial, Helvetica, sans-serif;
		font-style:			normal;	
		border-bottom:		1px solid #bbb;
		border-bottom-width: 	1px;
		background-image:   	none;
		overflow: 				hidden;
		border-bottom-style: 	solid;
		border-bottom-color: 	#bbbbbb;
		/*font-size: 			80%;*/
		empty-cells: 		show;
		
		
		empty-cells: 		show;
		/*white-space: 		nowrap;*/
		overflow-x: 		hidden;
		overflow-y: 		hidden;
		overflow:			hidden;
		
	}		

	.datatable tr.odd td,
	.datatable tr.odd {
		background:         #E2E2E2;
	}
	.datatable tr.even td,
	.datatable tr.even {
		background:         #EEEEEE;
	}
	
	.cMark {
		width: 30px;
		text-align:         right;
		/*max-width:			50px;*/
		/* border: 1px dashed red; */
	}
	
	.cStar {
		/*overflow:			hidden;*/
		width: 				18px;
		/* border: 1px dashed red; */
	}

	.cCid {
		width: 				150px;
		overflow: 			hidden;
		/* border: 1px dashed red; */
	}
	
	.cRev {
		/*width: 40%; */
		overflow: hidden;
		/* border: 1px dashed red; */
	}
	
	.cDate {
		width: 				90px;
		overflow: 			hidden;
		/* border: 1px dashed red; */
	}

	.cSpace {
		width: 10px;
		overflow: hidden;
		/* border: 1px dashed red; */
	}
	
	.tcp { /*table content preview*/
		color: #777777;
	}
	
	.cell-height {
		overflow:hidden;
		white-space:nowrap;
		/*height: 14px;*/
		display: block;
		vertical-align:middle;
		float:left;
	}
	
	.cStarOn {
		/*background:			url('images/star_on.gif') no-repeat left;
		display: 			block;*/
		border: 0px;
		background-x-position: 	1px;
		background-y-position: 	3px;
		background-repeat: 		no-repeat;
		background-image: 		url('images/star_on.gif');
		cursor: pointer;
	}
	
	.tabselect {
		/*font-size: 80%;	*/
		padding-top: 4px;
		padding-right: 0px;
		padding-bottom: 2px;
		padding-left: 0px;
		background-color: #c3d9ff;
	}
	
	.listxofy {
		float:right;
		text-align:right;
		padding-top:2px;
	}


-->
</style>

<table class="datatable" id="test" name="test" cellpadding="1" cellspacing="0">
	<thead>
		<tr class="head">
			<th scope="col" class="cMark">&nbsp;</th>
			<th scope="col" class="cStar">&nbsp;</th>
			<th scope="col" class="cCid">Name</th>
			<th scope="col" class="cSpace">&nbsp;</th>
			<th scope="col" class="cRev">Content</th>
			<th scope="col" class="cSpace">&nbsp;</th>
			<th scope="col" class="cDate">Date</th>
		</tr>
	</thead>
	<tbody>
		<tr class="odd" id="tr1">
			<td align="right"><input id="cb1" type="checkbox" onclick="selcb(this.id)" /></td>
			<td><div class="cStarOn">&nbsp;</div></td>
			<td>test1</td>
			<td>&nbsp;</td>
			<td><div class="cell-height">content <span class="tcp">Fgg y&uuml;q sadasjdjashd asd saidjaks askdj aksdajsdjasd jlasl jklsadl lasdlj las sajdj sadjsa dasldkasl k</span></div></td>
			<td >&nbsp;</td>
			<td>2007-02-19</td>
		</tr>
		<tr class="even" id="tr2">
			<td align="right"><input id="cb2" type="checkbox" onclick="selcb(this.id)" /></td>
			<td><div class="cStarOff">&nbsp;</div></td>
			<td>index</td>
			<td>&nbsp;</td>
			<td>page <span class="tcp">bla bla</span></td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<tr class="odd" id="tr3">
		  <td align="right"><input id="cb3" type="checkbox" onclick="selcb(this.id)" /></td>
			<td><div class="cStarOff">&nbsp;</div></td>
			<td>test3</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<tr class="even" id="tr4">
		  <td align="right"><input id="cb4" type="checkbox" onclick="selcb(this.id)" /></td>
			<td><div class="cStarOn">&nbsp;</div></td>
			<td>test4</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
	</tbody>
</table>



<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p><a href="javascript:select_all(1)">select all </a></p>
<p><a href="javascript:select_all(0)">deselect all </a></p>
<p><a href="javascript:do_something()">marked? </a></p>
<p><a href="javascript:select_stars(1)">select all starred</a></p>
<p><a href="javascript:select_stars(0)">deselect all starred</a></p>
<p><a href="javascript:select_nds('stable')">select stable</a></p>
<p><a href="javascript:select_nds('new')">select new</a></p>
<p><a href="javascript:select_nds('draft')">select draft</a></p>


<div id="filters">2</div>
<div id="edit">3</div>
<div id="diff">4</div>
<div id="list">5</div>
<script type="text/javascript">
	var nres=4;
	var hlRows=true;
	//var retrDC=true;
	window.setTimeout("add_js_extras()", 100);
</script>

