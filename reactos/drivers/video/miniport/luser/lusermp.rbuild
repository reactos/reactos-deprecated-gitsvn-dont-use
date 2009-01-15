<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../../../tools/rbuild/project.dtd">
<module name="lusermp" type="kernelmodedriver" installbase="system32/drivers" installname="vbemp.sys">
	<include base="lusermp">.</include>
        <if property="LUSER" value="1">
          <define name="LUSER"/>
        </if>
	<library>videoprt</library>
	<file>lusermp.c</file>
	<file>lusermp.rc</file>
</module>
