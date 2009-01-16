<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../../../tools/rbuild/project.dtd">
<module name="lusermp" type="kernelmodedriver" installbase="system32/drivers" installname="vbemp.sys">
	<include base="lusermp">.</include>
        <include base="ReactOS">include/reactos/libs/luser</include>
	<library>videoprt</library>
        <library>luser</library>
        <library>libcntpr</library>
	<file>lusermp.c</file>
	<file>lusermp.rc</file>
</module>
