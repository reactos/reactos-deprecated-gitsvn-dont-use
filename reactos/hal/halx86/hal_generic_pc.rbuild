<?xml version="1.0"?>
<!DOCTYPE group SYSTEM "../../../tools/rbuild/project.dtd">
<group>
	<module name="hal_generic_pc" type="objectlibrary">
		<include>include</include>
		<include base="ntoskrnl">include</include>
		<define name="_DISABLE_TIDENTS" />
		<define name="_NTHAL_" />
                <if property="LUSER" value="1">
                  <define name="LUSER" />
                </if>
		<directory name="generic">
			<file>pci.c</file>
		</directory>
	</module>
</group>
