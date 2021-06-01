// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CGMICCOLORPRESETS_HPP
#define CGMICCOLORPRESETS_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//----------------------------------//
//----- CGmicColorPresetsParam -----//
//----------------------------------//
class CGmicColorPresetsParam: public CGmicTaskParam
{
    public:

        enum NormalizeColors { NONE, PRE_NORMALIZE, POST_NORMALIZE, BOTH };

        CGmicColorPresetsParam() : CGmicTaskParam()
        {
        }

        void        setParamMap(const UMapString& paramMap) override
        {
            m_category = std::stoi(paramMap.at("category"));
            m_preset = std::stoi(paramMap.at("preset"));
            m_thumbSize = std::stoi(paramMap.at("thumbSize"));
            m_strenght = std::stod(paramMap.at("strenght"));
            m_brightness = std::stod(paramMap.at("brightness"));
            m_contrast = std::stod(paramMap.at("contrast"));
            m_gamma = std::stod(paramMap.at("gamma"));
            m_hue = std::stod(paramMap.at("hue"));
            m_saturation = std::stod(paramMap.at("saturation"));
            m_normalizeColors = std::stoi(paramMap.at("normalizeColors"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("category", std::to_string(m_category)));
            map.insert(std::make_pair("preset", std::to_string(m_preset)));
            map.insert(std::make_pair("thumbSize", std::to_string(m_thumbSize)));
            map.insert(std::make_pair("strenght", std::to_string(m_strenght)));
            map.insert(std::make_pair("brightness", std::to_string(m_brightness)));
            map.insert(std::make_pair("contrast", std::to_string(m_contrast)));
            map.insert(std::make_pair("gamma", std::to_string(m_gamma)));
            map.insert(std::make_pair("hue", std::to_string(m_hue)));
            map.insert(std::make_pair("saturation", std::to_string(m_saturation)));
            map.insert(std::make_pair("normalizeColors", std::to_string(m_normalizeColors)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            std::string strParameters = std::to_string(m_category) + ",";
            for(size_t i=0; i<m_categories.size(); ++i)
            {
                if(i == (size_t)m_category)
                    strParameters += std::to_string(m_preset) + ",";
                else
                    strParameters += "1,";
            }
            strParameters += std::to_string(m_thumbSize) + "," + std::to_string(m_strenght) + "," +
                    std::to_string(m_brightness) + "," + std::to_string(m_contrast) + "," + std::to_string(m_gamma) + "," +
                    std::to_string(m_hue) + "," + std::to_string(m_saturation) + "," + std::to_string(m_normalizeColors);
            return strParameters;
        }

    public:

        const std::vector<std::string> m_categories =
        {
            "Abigail Gonzalez(21)", "Alex Jordan(81)", "Creative Pack(33)", "Eric Ellerbrock(14)", "FilterGrade Cinematic(8)", "J.T. Semple(14)",
            "Lutify.Me(7)", "Moviz(48)", "Ohad Peretz(7)", "ON1 Photography(90)", "PictureFX(19)", "PIXLS.US(31)", "RocketStock(35)",
            "Shamoon Abbasi(25)", "SmallHD Movie Look(7)", "Others (69)"
        };

        const std::vector<std::vector<std::string>> m_presets =
        {
            { "All[Collage]", "None", "Blade Runner", "Blue House", "Blue Ice", "Caribe", "Cinema", "Cinema 2", "Cinema 3", "Cinema 4", "Cinema 5",
              "Cinema Noir", "Cinematic for Flog", "Day4Nite", "Eterna for Flog", "Filmic", "Fuji HDR", "Golden Gate", "Matrix", "Monochrome 1",
              "Monochrome 2","Old West","Science Fiction" },
            { "All[Collage]", "None", "Action Magenta 01", "Action Red 01", "Adventure 1453", "Aggressive Highlights Recovery 5", "Bleech Bypass Green",
              "Bleech Bypass Yellow 01", "Blue Dark", "Blue Shadows 01", "Bright Green 01", "Brownish", "Colorful 0209", "Conflict 01",
              "Contrast with Highlights Protection", "Contrasty Afternoon", "Contrasty Green", "Cross Process CP 130", "Cross Process CP 14",
              "Cross Process CP 15", "Cross Process CP 16", "Cross Process CP 18", "Cross Process CP 3", "Cross Process CP 4", "Cross Process CP 6",
              "Dark Green 02", "Dark Green 1", "Dark Place 01", "Dream 1", "Dream 85", "Faded Retro 01", "Faded Retro 02", "Film 0987", "Film 9879",
              "Film Highlight Contrast", "Flat 30", "Green 2025", "Green Action", "Green Afternoon", "Green Conflict", "Green Day 01", "Green Day 02",
              "Green G09", "Green Indoor", "Green Light", "Harsh Day", "Harsh Sunset", "Highlights Protection", "Indoor Blue", "Low Contrast Blue",
              "Low Key 01", "Magenta Day", "Magenta Day 01", "Magenta Dream", "Memories", "Moonlight 01", "Mostly Blue", "Muted 01", "Night 01",
              "Only Red", "Only Red and Blue", "Operation Yellow", "Orange Dark 4", "Orange Dark 7", "Orange Dark Look", "Orange Underexposed",
              "Protect Highlights 01", "Red Afternoon 01", "Red Day 01", "Red Dream 01", "Retro Brown 01", "Retro Magenta 01", "Retro Yellow 01",
              "Saturated Blue", "Smart Contrast", "Subtle Blue", "Subtle Green", "Yellow 55B", "Yellow Film 01" },
            { "All[Collage]", "None", "Anime", "Bleach Bypass 1", "Bleach Bypass 2", "Bleach Bypass 3", "Bleach Bypass 4", "Candle Light",
              "Color Negative", "Crisp Warm", "Crip Winter", "Drop Blues", "Edgy Ember", "Fall Colors", "Foggy Night", "Futuristic Bleak 1",
              "Futuristic Bleak 2", "Futuristic Bleak 3", "Futuristic Bleak 4", "Horror Blue", "Late Sunset", "Moonlight", "Night From Day",
              "Red Blue Yellow", "Smokey", "Soft Warming", "Teal Magenta Gold", "Teal Orange", "Teal Orange 1", "Teal Orange 2", "Teal Orange 3",
              "Tension Green 1", "Tension Green 2", "Tension Green 3","Tension Green 4" },
            { "All[Collage]", "None", "Avalanche", "Black Star", "Helios", "Hydracore", "Hypnosis", "Killstreak", "Nemesis", "Night Blade 4",
              "Paladin", "Seringe 4", "Serpent", "Terra 4", "Victory", "Yellowstone" },
            { "All[Collage]", "None", "Cine Basic", "Cine Bright", "Cine Cold", "Cine Drama", "Cine Teal Orange 1", "Cine Teal Orange 2",
              "Cine Vibrant", "Cine Warm" },
            { "All[Collage]", "None", "Bright Green", "Crisp Romance", "Crushin", "Frosted Beach Picnic", "Just Peachy", "Late Afternoon Wanderlust",
              "Lush Green Summer", "Magenta Coffee", "Minimalist Caffeination", "Mystic Purple Sunset", "Nostalgia Honey", "Spring Morning",
              "Toasted Garden", "Winter Lighthouse" },
            { "All[Collage]", "None", "Hackmanite", "Herderite", "Heulandite", "Hiddenite", "Hilutite", "Howlite", "Hypersthene" },
            { "All[Collage]", "None", "Moviz 1", "Moviz 2", "Moviz 3", "Moviz 4", "Moviz 5", "Moviz 6", "Moviz 7", "Moviz 8", "Moviz 9", "Moviz 10",
              "Moviz 11", "Moviz 12", "Moviz 13", "Moviz 14", "Moviz 15", "Moviz 16", "Moviz 17", "Moviz 18", "Moviz 19", "Moviz 20", "Moviz 21",
              "Moviz 22", "Moviz 23", "Moviz 24", "Moviz 25", "Moviz 26", "Moviz 27", "Moviz 28", "Moviz 29", "Moviz 30", "Moviz 31", "Moviz 32",
              "Moviz 33", "Moviz 34", "Moviz 35", "Moviz 36", "Moviz 37", "Moviz 38", "Moviz 39", "Moviz 40", "Moviz 41", "Moviz 42", "Moviz 43",
              "Moviz 44", "Moviz 45", "Moviz 46", "Moviz 47", "Moviz 48" },
            { "All[Collage]", "None", "Cold Simplicity 2", "D and O 1", "Retro Summer 3", "Subtle Yellow", "Teal Moonlight", "True Colors 8", "Vintage Warmth 1" },
            { "All[Collage]", "None", "2-Strip Process", "Aqua", "Aqua and Orange Dark", "Berlin Sky", "Blues", "Black & White-1", "Black & White-2",
              "Black & White-3", "Black & White-4", "Black & White-5", "Black & White-6", "Black & White-7", "Black & White-8", "Black & White-9",
              "Black & White-10", "Chrome 01", "Cinematic-1", "Cinematic-2", "Cinematic-3", "Cinematic-4", "Cinematic-5", "Cinematic-6", "Cinematic-7",
              "Cinematic-8", "Cinematic-9", "Cinematic-10", "Classic Teal and Orange", "Earth Tone Boost", "Fade to Green", "Film Print 01", "Film Print 02",
              "French Comedy", "Green Blues", "Green Yellow", "Landscape-1", "Landscape-2", "Landscape-3", "Landscape-4", "Landscape-5", "Landscape-6",
              "Landscape-7", "Landscape-8", "Landscape-9", "Landscape-10", "Lifestyle & Commercial-1", "Lifestyle & Commercial-2", "Lifestyle & Commercial-3",
              "Lifestyle & Commercial-4", "Lifestyle & Commercial-5", "Lifestyle & Commercial-6", "Lifestyle & Commercial-7", "Lifestyle & Commercial-8",
              "Lifestyle & Commercial-9", "Lifestyle & Commercial-10", "Moody-1", "Moody-2", "Moody-3", "Moody-4", "Moody-5", "Moody-6", "Moody-7", "Moody-8",
              "Moody-9", "Moody-10", "Nature & Wildlife-1", "Nature & Wildlife-2", "Nature & Wildlife-3", "Nature & Wildlife-4", "Nature & Wildlife-5",
              "Nature & Wildlife-6", "Nature & Wildlife-7", "Nature & Wildlife-8", "Nature & Wildlife-9", "Nature & Wildlife-10", "Oranges", "Portrait-1",
              "Portrait-2", "Portrait-3", "Portrait-4", "Portrait-5", "Portrait-6", "Portrait-7", "Portrait-8", "Portrait-9", "Portrait10", "Purple", "Reds",
              "Reds Oranges Yellows", "Studio Skin Tone Shaper", "Vintage Chrome" },
            { "All[Collage]", "None", "AnalogFX - Anno 1870 Color", "AnalogFX - Old Style I", "AnalogFX - Old Style II", "AnalogFX - Old Style III",
              "AnalogFX - Sepia Color", "AnalogFX - Soft Sepia I", "AnalogFX - Soft Sepia II", "GoldFX - Bright Spring Breeze", "GoldFX - Bright Summer Heat",
              "GoldFX - Hot Summer Heat", "GoldFX - Perfect Sunset 01min", "GoldFX - Perfect Sunset 05min", "GoldFX - Perfect Sunset 10min",
              "GoldFX - Spring Breeze","GoldFX - Summer Heat", "TechnicalFX - Backlight Filter", "ZilverFX - B&amp;W Solarization", "ZilverFX - InfraRed",
              "ZilverFX - Vintage B&amp;W" },
            { "All[Collage]", "None", "Amstragram", "Amstragram+", "Autumn", "Cinematic Lady Bird", "Cinematic Mexico", "Dark Blues in Sunlight", "Delicatessen",
              "Expired 69", "Faded Look", "Faded Print", "Hypressen", "Magenta Yellow", "Metropolis", "Modern Film", "Newspaper", "Night Spy", "Progressen",
              "Prussian Blue", "Seventies Magazine", "Street", "Sweet Bubblegum", "Sweet Gelatto", "Taiga", "Tarraco", "Unknown", "Uzbek Bukhara",
              "Uzbek Marriage", "Uzbek Samarcande", "Velvetia", "Warm Vintage", "Whiter Whites" },
            { "All[Collage]", "None", "Arabica 12", "Ava 614", "Azrael 93", "Bourbon 64", "Byers 11", "Chemical 168", "Clayton 33", "Clouseau 54", "Cobi 3",
              "Contrail 35", "Cubicle 99", "Django 25", "Domingo 145", "Faded 47", "Folger 50", "Fusion 88", "Hyla 68", "Korben 214", "Lenox 340", "Lucky 64",
              "McKinnon 75", "Milo 5", "Neon 770", "Paladin 1875", "Pasadena 21", "Pitaya 15", "Reeve 38", "Remy 24", "Sprocket 231", "Teigen 28", "Trent 18",
              "Tweed 71", "Vireo 37", "Zed 32", "Zeke 39" },
            { "All[Collage]", "None", "City 7", "Coffee 44", "Date 39", "Day for Night", "Denoise Simple 40", "Desert Gold 37", "Directions 23",
              "Drop Green Tint 14", "Elegance 38", "Golden Night Softner 43", "Golden Sony 37", "Green 15", "Happyness 133", "HLG 1", "Industrial 33",
              "Morning 6", "Morroco 16", "Night King 141", "Rest 33", "Shadow King 39", "Spy 29", "Thriller 2", "Turkiest 42", "Vintage 163", "Wooden Gold 20" },
            { "All[Collage]", "None", "Apocalypse This Very Moment", "B-Boyz 2", "Bob Ford", "Life Giving Tree", "Moonrise", "Saving Private Damon", "The Matrices" },
            { "All[Collage]", "None", "60's", "60's (faded)", "60's (faded alt)", "Alien green", "Black &amp; White", "Bleach bypass", "Blue mono", "Cinematic-01",
              "Cinematic-02", "Cinematic-03", "Color (rich)", "Faded", "Faded (alt)", "Faded (analog)", "Faded (extreme)", "Faded (vivid)", "Expired (fade)",
              "Expired (polaroid)", "Extreme", "Fade", "Faux infrared", "Golden", "Golden (bright)", "Golden (fade)", "Golden (mono)", "Golden (vibrant)",
              "Green mono", "Hong Kong", "Instant-C", "K-Tone Vintage Kodachrome", "Light (blown)", "Lomo", "Mono tinted", "Mute shift", "Natural (vivid)",
              "Nostalgic", "Orange tone", "Pink fade", "Purple", "Retro", "Rotate (muted)", "Rotate (vibrant)", "Rotated", "Rotated (crush)", "Smooth crome-ish",
              "Smooth fade", "Soft fade", "Solarize color", "Solarized color2", "Summer", "Summer (alt)", "Sunny", "Sunny (alt)", "Sunny (warm)", "Sunny (rich)",
              "Super warm", "Super warm (rich)", "Sutro FX", "Vibrant", "Vibrant (alien)", "Vibrant (contrast)", "Vibrant (crome-ish)", "Vintage", "Vintage (alt)",
              "Vintage (brighter)", "Warm", "Warm (highlight)", "Warm (yellow)" }
        };

        int     m_category = 0;
        int     m_preset = 1;
        int     m_thumbSize = 512;
        float   m_strenght = 100;
        float   m_brightness = 0;
        float   m_contrast = 0;
        float   m_gamma = 0;
        float   m_hue = 0;
        float   m_saturation = 0;
        int     m_normalizeColors = NONE;
};

//-----------------------------//
//----- CGmicColorPresets -----//
//-----------------------------//
class CGmicColorPresets : public CImageProcess2d
{
    public:

        CGmicColorPresets() : CImageProcess2d()
        {
        }
        CGmicColorPresets(const std::string name, const std::shared_ptr<CGmicColorPresetsParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicColorPresetsParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<CGmicColorPresetsParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst(imgSrc.rows, imgSrc.cols, imgSrc.type());
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Put image inputs into a gmic_list object
                gmic_list<float> imageList;
                gmic_list<char> image_names;

                //Allocate list, parameter is the number of image inputs
                imageList.assign(1);
                //Conversion from CMat(cv::Mat) to gmic_image(CImg)
                CGmicImageConverter::convert(imgSrc, imageList[0]);
                //Build command for gmic interpreter
                std::string command = "fx_color_presets[0] " + pParam->getParamString();
                //Call interpreter
                gmic(command.c_str(), imageList, image_names);
                //Conversion from gmic_image to CMat
                CGmicImageConverter::convert(imageList[0], imgDst);
                //Free memory
                imageList.assign(0);
            }
            catch(gmic_exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CGmicColorPresetsFactory : public CProcessFactory
{
    public:

        CGmicColorPresetsFactory()
        {
            m_info.m_name = QObject::tr("Color presets").toStdString();
            m_info.m_description = QObject::tr("This process applies pre-defined LUT on images.").toStdString();
            m_info.m_docLink = "https://gmic.eu/reference.shtml#remove_ColorPresets";
            m_info.m_path = QObject::tr("Gmic/Colors").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "color,lut,colormap";
            m_info.m_authors = "David Tschumperlé";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicColorPresetsParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicColorPresets>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicColorPresetsParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicColorPresets>(m_info.m_name, pParam);
        }
};

#endif // CGMICCOLORPRESETS_HPP
