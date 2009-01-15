<?xml version="1.0"?>
<!DOCTYPE group SYSTEM "../../../tools/rbuild/project.dtd">
<group xmlns:xi="http://www.w3.org/2001/XInclude">
  <if property="LUSER" value="1">
        <directory name="luser">
                <xi:include href="luser/lusermp.rbuild" />
        </directory>
  </if>
  <if property="LUSER" value="0">
	<directory name="vbe">
		<xi:include href="vbe/vbemp.rbuild" />
	</directory>
  </if>
	<directory name="vga">
		<xi:include href="vga/vgamp.rbuild" />
	</directory>
	<directory name="xboxvmp">
		<xi:include href="xboxvmp/xboxvmp.rbuild" />
	</directory>
</group>
