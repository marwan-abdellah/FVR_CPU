#include <osg/Node>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/Texture3D>
#include <osg/Texture1D>
#include <osg/ImageSequence>
#include <osg/TexGen>
#include <osg/Geode>
#include <osg/Billboard>
#include <osg/PositionAttitudeTransform>
#include <osg/ClipNode>
#include <osg/AlphaFunc>
#include <osg/TexGenNode>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/Material>
#include <osg/PrimitiveSet>
#include <osg/Endian>
#include <osg/BlendFunc>
#include <osg/BlendEquation>
#include <osg/TransferFunction>
#include <osg/MatrixTransform>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgGA/EventVisitor>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/KeySwitchMatrixManipulator>

#include <osgUtil/CullVisitor>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/TrackballDragger>

#include <osg/io_utils>

#include <algorithm>
#include <iostream>

#include <osg/ImageUtils>
#include <osgVolume/Volume>
#include <osgVolume/VolumeTile>
#include <osgVolume/RayTracedTechnique>
#include <osgVolume/FixedFunctionTechnique>



osg::Image* readRaw(int sizeX, int sizeY, int sizeZ,
                    int numberBytesPerComponent,
                    int numberOfComponents,
                    const std::string& endian,
                    const std::string& raw_filename)
{
    // Input file stream for a binary file
    osgDB::ifstream fin(raw_filename.c_str(), std::ifstream::binary);
    if (!fin)
        return 0;

    // Pixel format (number of color channels or components)
    GLenum pixelFormat;
    switch(numberOfComponents)
    {
        case 1 :
            pixelFormat = GL_LUMINANCE;
            break;
        case 2 :
            pixelFormat = GL_LUMINANCE_ALPHA;
            break;
        case 3 :
            pixelFormat = GL_RGB;
            break;
        case 4 :
            pixelFormat = GL_RGBA;
            break;
        default :
            osg::notify(osg::NOTICE)<<"Error: numberOfComponents="<<numberOfComponents<<" not supported, only 1,2,3 or 4 are supported."<<std::endl;
            return 0;
    }

    // Number of bytes per channel or component
    GLenum dataType;
    switch(numberBytesPerComponent)
    {
        case 1 :
            dataType = GL_UNSIGNED_BYTE;
            break;
        case 2 :
            dataType = GL_UNSIGNED_SHORT;
            break;
        case 4 :
            dataType = GL_UNSIGNED_INT;
            break;
        default :
            osg::notify(osg::NOTICE)<<"Error: numberBytesPerComponent="<<numberBytesPerComponent<<" not supported, only 1,2 or 4 are supported."<<std::endl;
            return 0;
    }

    int s_maximumTextureSize=256, t_maximumTextureSize=256, r_maximumTextureSize=256;

    int sizeS = sizeX;
    int sizeT = sizeY;
    int sizeR = sizeZ;

    // Power of two
    clampToNearestValidPowerOfTwo(sizeS, sizeT, sizeR, s_maximumTextureSize, t_maximumTextureSize, r_maximumTextureSize);

    // Image object to hold the volume data
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->allocateImage(sizeS, sizeT, sizeR, pixelFormat, dataType);

    // Arrangement
    bool endianSwap = (osg::getCpuByteOrder()==osg::BigEndian) ? (endian!="big") : (endian=="big");

    // Offset
    unsigned int r_offset = (sizeZ<sizeR) ? sizeR/2 - sizeZ/2 : 0;

    int offset = endianSwap ? numberBytesPerComponent : 0;
    int delta = endianSwap ? -1 : 1;
    for(int r=0;r<sizeZ;++r)
    {
        for(int t=0;t<sizeY;++t)
        {
            char* data = (char*) image->data(0,t,r+r_offset);
            for(int s=0;s<sizeX;++s)
            {
                if (!fin) return 0;

                for(int c=0;c<numberOfComponents;++c)
                {
                    char* ptr = data+offset;
                    for(int b=0;b<numberBytesPerComponent;++b)
                    {
                        fin.read((char*)ptr, 1);
                        ptr += delta;
                    }
                    data += numberBytesPerComponent;
                }
            }
        }
    }


    // Normalize texture
    {
        // compute range of values
        osg::Vec4 minValue, maxValue;
        osg::computeMinMax(image.get(), minValue, maxValue);
        osg::modifyImage(image.get(),ScaleOperator(1.0f/maxValue.r()));
    }

    // Close the input stream
    fin.close();

    if (dataType!=GL_UNSIGNED_BYTE)
    {
        // need to convert to ubyte

        osg::ref_ptr<osg::Image> new_image = new osg::Image;
        new_image->allocateImage(sizeS, sizeT, sizeR, pixelFormat, GL_UNSIGNED_BYTE);

        RecordRowOperator readOp(sizeS);
        WriteRowOperator writeOp;

        for(int r=0;r<sizeR;++r)
        {
            for(int t=0;t<sizeT;++t)
            {
                // reset the indices to beginning
                readOp._pos = 0;
                writeOp._pos = 0;

                // read the pixels into readOp's _colour array
                osg::readRow(sizeS, pixelFormat, dataType, image->data(0,t,r), readOp);

                // pass readOp's _colour array contents over to writeOp (note this is just a pointer swap).
                writeOp._colours.swap(readOp._colours);

                osg::modifyRow(sizeS, pixelFormat, GL_UNSIGNED_BYTE, new_image->data(0,t,r), writeOp);

                // return readOp's _colour array contents back to its rightful owner.
                writeOp._colours.swap(readOp._colours);
            }
        }

        image = new_image;
    }

    // Return the image
    return image.release();


}
