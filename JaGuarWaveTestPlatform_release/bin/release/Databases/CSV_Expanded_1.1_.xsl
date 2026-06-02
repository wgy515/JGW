<!--
/*!
 *********************************************************************
 *  @file   : CSV#Expanded#1.1#.xsl
 *
 *  Project : QCT SUITE
 *
 *  Package : XMLDataLogger
 *
 *  Company : Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
 *
 *  Purpose : Stylesheet for generating csv output from XML source.
 *
 *********************************************************************
 * Version History:
 *
 * V0.1   |  3/11/2002   |  First Revision - Updated from V1.1 Uses Extended Test Name
 *                               
 *
 *********************************************************************
 */
-->

<!-- Requirement for use:  all tests must contain the exact same indexed parameters.  Order does not matter,
     but if you have a total of five indexed parameters, they better be logged in each data set 
-->

<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
<xsl:output method="text" encoding="utf-8"/>

<xsl:template match="source">

	<!-- Print header row -->
	<xsl:text>UUT_ID,Test Name,</xsl:text>
	<xsl:for-each select="TestCollection[last()]/Test[1]/DataSetCollection[1]/DataSet[1]/Inputs/DI[@I]">
		<xsl:sort select="." data-type="number"/>
		<xsl:value-of select="N"/><xsl:text>,</xsl:text>
	</xsl:for-each>
	<xsl:text>Conditions,ResultName,Value,,Min,Max</xsl:text><xsl:text>&#xA;</xsl:text>

	<xsl:for-each select="TestCollection[last()]/Test">
		<xsl:if test="count(@LogPriority)=0 or (@LogPriority &lt; 10)">
			<xsl:for-each select="DataSetCollection">
				<xsl:for-each select="DataSet">
					<xsl:for-each select="Outputs/Result">
						<xsl:if test="count(@LogPriority)=0 or (@LogPriority &lt; 10)">
		
							<xsl:value-of select="../../../../../UUT/ID"/><xsl:text>,</xsl:text>	<!-- UUT ID -->
							<xsl:value-of select="../../../../ExtendedName"/><xsl:text>,</xsl:text>	<!-- Test Name -->
			
							<!-- Print indexed parameters -->
							<xsl:for-each select="../../Inputs/DI[@I]">
								<xsl:sort select="." data-type="number"/>
								<xsl:value-of select="V"/><xsl:text>,</xsl:text>
							</xsl:for-each>

							<xsl:for-each select="../../Inputs/DI">   <!-- And All inputs -->
								<xsl:if test="count(@I)=0 and (count(@LogPriority)=0 or (@LogPriority &lt; 10))">
									<xsl:value-of select="N"/><xsl:text>=</xsl:text>
									<xsl:value-of select="V"/>
									<xsl:value-of select="U"/>
									<xsl:if test="not(position()=last())">
										<xsl:text>_</xsl:text>
									</xsl:if>
								</xsl:if>
							</xsl:for-each>
							
							<!-- Print the result name -->
							<xsl:text>,</xsl:text>
							<xsl:value-of select="DI/N"/>
							<xsl:if test="not(count(DI/U)=0)">
								<xsl:text>(</xsl:text><xsl:value-of select="DI/U"/><xsl:text>)</xsl:text>
							</xsl:if>

							<!-- Print the result value -->
							<xsl:text>,</xsl:text>
							<xsl:value-of select="DI/V"/>

							<!-- Print the min and max -->
							<xsl:text>,,</xsl:text>
							<xsl:value-of select="Limits/Min"/>
							<xsl:text>,</xsl:text>
							<xsl:value-of select="Limits/Max"/>					
							<xsl:text>&#xA;</xsl:text>
						</xsl:if>
					</xsl:for-each>

				</xsl:for-each> <!-- DataSet -->
			</xsl:for-each> <!-- DataSetCollection -->
		</xsl:if>
	</xsl:for-each> <!-- Test -->

</xsl:template>
</xsl:stylesheet>
