
HHC=C:\Programme\HTML Help Workshop\hhc

XSLT=\docbook\xsltproc\xsltproc

XSLPATH=../docbook-xsl-1.45

RM=del
#RM=rm


html:
	-$(XSLT) $(XSLPATH)/html/chunk.xsl tutorials/tutorials.xml

htmlhelp:
	-$(XSLT) $(XSLPATH)/htmlhelp/htmlhelp.xsl tutorials/tutorials.xml
	-$(HHC) htmlhelp.hhp

clean:
	-$(RM) *.html
	-$(RM) *.hhp *.chm *.hhc