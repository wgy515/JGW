// QRencode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "qrencode.h"
#include <string.h>
#include <stdlib.h>
static int casesensitive = 1;
static int eightbit = 0;
static int version = 0;
static int size = 3;
static int margin = 5;
static int dpi = 72;
static int structured = 0;
static int rle = 0;
static int svg_path = 0;
static int micro = 0;
static QRecLevel level = QR_ECLEVEL_L;
static QRencodeMode hint = QR_MODE_8;
static unsigned char fg_color[4] = {0, 0, 0, 255};
static unsigned char bg_color[4] = {255, 255, 255, 255};

static int verbose = 0;
//
static QRcode *encode(const unsigned char *intext, int length)
{
    QRcode *code;

    if(micro) {
        if(eightbit) {
            code = QRcode_encodeDataMQR(length, intext, version, level);
        } else {
            code = QRcode_encodeStringMQR((char *)intext, version, level, hint, casesensitive);
        }
    } else if(eightbit) {
        code = QRcode_encodeData(length, intext, version, level);
    } else {
        code = QRcode_encodeString((char *)intext, version, level, hint, casesensitive);
    }

    return code;
}
//
//static void qrencode(const unsigned char *intext, int length, const char *outfile)
//{
//    QRcode *qrcode;
//
//    qrcode = encode(intext, length);
//    if(qrcode == NULL) {
//        if(errno == ERANGE) {
//            fprintf(stderr, "Failed to encode the input data: Input data too large\n");
//        } else {
//            perror("Failed to encode the input data");
//        }
//        exit(EXIT_FAILURE);
//    }
//
//    if(verbose) {
//        fprintf(stderr, "File: %s, Version: %d\n", (outfile!=NULL)?outfile:"(stdout)", qrcode->version);
//    }
//
//    switch(image_type) {
//    case PNG_TYPE:
//    case PNG32_TYPE:
//        writePNG(qrcode, outfile, image_type);
//        break;
//    case EPS_TYPE:
//        writeEPS(qrcode, outfile);
//        break;
//    case SVG_TYPE:
//        writeSVG(qrcode, outfile);
//        break;
//    case XPM_TYPE:
//        writeXPM(qrcode, outfile);
//        break;
//    case ANSI_TYPE:
//    case ANSI256_TYPE:
//        writeANSI(qrcode, outfile);
//        break;
//    case ASCIIi_TYPE:
//        writeASCII(qrcode, outfile,  1);
//        break;
//    case ASCII_TYPE:
//        writeASCII(qrcode, outfile,  0);
//        break;
//    case UTF8_TYPE:
//        writeUTF8(qrcode, outfile, 0, 0);
//        break;
//    case ANSIUTF8_TYPE:
//        writeUTF8(qrcode, outfile, 1, 0);
//        break;
//    case UTF8i_TYPE:
//        writeUTF8(qrcode, outfile, 0, 1);
//        break;
//    case ANSIUTF8i_TYPE:
//        writeUTF8(qrcode, outfile, 1, 1);
//        break;
//    default:
//        fprintf(stderr, "Unknown image type.\n");
//        exit(EXIT_FAILURE);
//    }
//
//    QRcode_free(qrcode);
//}

#define INCHES_PER_METER (100.0/2.54)
#define snprintf _snprintf
static FILE *openFile(const char *outfile)
{
    FILE *fp;

    if(outfile == NULL || (outfile[0] == '-' && outfile[1] == '\0')) {
        fp = stdout;
    } else {
        fp = fopen(outfile, "wb");
        if(fp == NULL) {
            fprintf(stderr, "Failed to create file: %s\n", outfile);
            perror(NULL);
            exit(1);
        }
    }

    return fp;
}

static void writeSVG_drawModules(FILE *fp, int x, int y, int width, const char* col, float opacity)
{
    if(svg_path) {
        fprintf(fp, "M%d,%dh%d", x, y, width);
    } else {
        if(fg_color[3] != 255) {
            fprintf(fp, "\t\t\t<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"1\" "\
                "fill=\"#%s\" fill-opacity=\"%f\"/>\n",
                x, y, width, col, opacity );
        } else {
            fprintf(fp, "\t\t\t<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"1\" "\
                "fill=\"#%s\"/>\n",
                x, y, width, col );
        }
    }
}

static int writeSVG(const QRcode *qrcode, const char *outfile)
{
	FILE *fp;
	unsigned char *row, *p;
	int x, y, x0, pen;
	int symwidth, realwidth;
	float scale;
	char fg[7], bg[7];
	float fg_opacity;
	float bg_opacity;

	fp = openFile(outfile);

	scale = dpi * INCHES_PER_METER / 100.0;

	symwidth = qrcode->width + margin * 2;
	realwidth = symwidth * size;

	snprintf(fg, 7, "%02x%02x%02x", fg_color[0], fg_color[1],  fg_color[2]);
	snprintf(bg, 7, "%02x%02x%02x", bg_color[0], bg_color[1],  bg_color[2]);
	fg_opacity = (float)fg_color[3] / 255;
	bg_opacity = (float)bg_color[3] / 255;

	/* XML declaration */
	fputs( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n", fp );

	/* DTD
	   No document type specified because "while a DTD is provided in [the SVG]
	   specification, the use of DTDs for validating XML documents is known to
	   be problematic. In particular, DTDs do not handle namespaces gracefully.
	   It is *not* recommended that a DOCTYPE declaration be included in SVG
	   documents."
	   http://www.w3.org/TR/2003/REC-SVG11-20030114/intro.html#Namespace
	*/

	/* Vanity remark */
	fprintf(fp, "<!-- Created with qrencode %s (https://fukuchi.org/works/qrencode/index.html) -->\n", QRcode_APIVersionString());

	/* SVG code start */
	fprintf(fp,
			"<svg width=\"%.2fcm\" height=\"%.2fcm\" viewBox=\"0 0 %d %d\""\
			" preserveAspectRatio=\"none\" version=\"1.1\""\
			" xmlns=\"http://www.w3.org/2000/svg\">\n",
			realwidth / scale, realwidth / scale, symwidth, symwidth
		   );

	/* Make named group */
	fputs("\t<g id=\"QRcode\">\n", fp);

	/* Make solid background */
	if(bg_color[3] != 255) {
		fprintf(fp, "\t\t<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"#%s\" fill-opacity=\"%f\"/>\n", symwidth, symwidth, bg, bg_opacity);
	} else {
		fprintf(fp, "\t\t<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"#%s\"/>\n", symwidth, symwidth, bg);
	}

	if(svg_path) {
		if(fg_color[3] != 255) {
			fprintf(fp, "\t\t<path style=\"stroke:#%s;stroke-opacity:%f\" transform=\"translate(%d,%d.5)\" d=\"", fg, fg_opacity, margin, margin);
		} else {
			fprintf(fp, "\t\t<path style=\"stroke:#%s\" transform=\"translate(%d,%d.5)\" d=\"", fg, margin, margin);
		}
	} else {
		/* Create new viewbox for QR data */
		fprintf(fp, "\t\t<g id=\"Pattern\" transform=\"translate(%d,%d)\">\n", margin, margin);
	}

	/* Write data */
	p = qrcode->data;
	for(y = 0; y < qrcode->width; y++) {
		row = (p+(y*qrcode->width));

		if( !rle ) {
			/* no RLE */
			for(x = 0; x < qrcode->width; x++) {
				if(*(row+x)&0x1) {
					writeSVG_drawModules(fp, x, y, 1, fg, fg_opacity);
				}
			}
		} else {
			/* simple RLE */
			pen = 0;
			x0  = 0;
			for(x = 0; x < qrcode->width; x++) {
				if( !pen ) {
					pen = *(row+x)&0x1;
					x0 = x;
				} else if(!(*(row+x)&0x1)) {
					writeSVG_drawModules(fp, x0, y, x-x0, fg, fg_opacity);
					pen = 0;
				}
			}
			if( pen ) {
				writeSVG_drawModules(fp, x0, y, qrcode->width - x0, fg, fg_opacity);
			}
		}
	}

	if(svg_path) {
		fputs("\"/>\n", fp);
	} else {
		/* Close QR data viewbox */
		fputs("\t\t</g>\n", fp);
	}

	/* Close group */
	fputs("\t</g>\n", fp);

	/* Close SVG code */
	fputs("</svg>\n", fp);
	fclose(fp);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    QRcode *qrcode;
    char* buf = "SN123456789Release Note: Three new output format, SVG, UTF8, and ANSIUTF8 have been added to the command line tool. UTF8 and ANSIUTF8 are another text art mode, using Unicode block elements for high-resolution text output. Long-awaited colored QR code has been introduced. Try \"--foreground\" and \"--background\" options to set the colors. Currently PNG and SVG supports colored output.";
    qrcode = encode((unsigned char*)buf, strlen(buf));
    writeSVG(qrcode,"d:\\123.svg");
    QRcode_free(qrcode);
    //qrencode(intext, length, outfile);
	return 0;
}

