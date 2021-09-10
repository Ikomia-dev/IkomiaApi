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

#include "CTaskInfo.h"
#include "UtilsTools.hpp"

CTaskInfo::CTaskInfo()
{
    m_ikomiaVersion = Utils::IkomiaApp::getCurrentVersionNumber().toStdString();
}

std::string CTaskInfo::getName() const
{
    return m_name;
}

std::string CTaskInfo::getPath() const
{
    return m_path;
}

std::string CTaskInfo::getShortDescription() const
{
    return m_shortDescription;
}

std::string CTaskInfo::getDescription() const
{
    return m_description;
}

std::string CTaskInfo::getDocumentationLink() const
{
    return m_docLink;
}

std::string CTaskInfo::getIconPath() const
{
    return m_iconPath;
}

std::string CTaskInfo::getKeywords() const
{
    return m_keywords;
}

std::string CTaskInfo::getAuthors() const
{
    return m_authors;
}

std::string CTaskInfo::getArticle() const
{
    return m_article;
}

std::string CTaskInfo::getJournal() const
{
    return m_journal;
}

std::string CTaskInfo::getVersion() const
{
    return m_version;
}

std::string CTaskInfo::getIkomiaVersion() const
{
    return m_ikomiaVersion;
}

std::string CTaskInfo::getCreatedDate() const
{
    return m_createdDate;
}

std::string CTaskInfo::getModifiedDate() const
{
    return m_modifiedDate;
}

std::string CTaskInfo::getLicense() const
{
    return m_license;
}

std::string CTaskInfo::getRepository() const
{
    return m_repo;
}

int CTaskInfo::getYear() const
{
    return m_year;
}

ApiLanguage CTaskInfo::getLanguage() const
{
    return m_language;
}

int CTaskInfo::getOS() const
{
    return m_os;
}

bool CTaskInfo::isInternal() const
{
    return m_bInternal;
}

void CTaskInfo::setName(const std::string &name)
{
    m_name = name;
}

void CTaskInfo::setPath(const std::string &path)
{
    m_path = path;
}

void CTaskInfo::setShortDescription(const std::string &description)
{
    m_shortDescription = description;
}

void CTaskInfo::setDescription(const std::string &description)
{
    m_description = description;
}

void CTaskInfo::setDocumentationLink(const std::string &link)
{
    m_docLink = link;
}

void CTaskInfo::setIconPath(const std::string &path)
{
    m_iconPath = path;
}

void CTaskInfo::setKeywords(const std::string &keywords)
{
    m_keywords = keywords;
}

void CTaskInfo::setAuthors(const std::string &authors)
{
    m_authors = authors;
}

void CTaskInfo::setArticle(const std::string &article)
{
    m_article = article;
}

void CTaskInfo::setJournal(const std::string &journal)
{
    m_journal = journal;
}

void CTaskInfo::setVersion(const std::string &version)
{
    m_version = version;
}

void CTaskInfo::setCreatedDate(const std::string &date)
{
    m_createdDate = date;
}

void CTaskInfo::setModifiedDate(const std::string &date)
{
    m_modifiedDate = date;
}

void CTaskInfo::setLicense(const std::string& license)
{
    m_license = license;
}

void CTaskInfo::setRepository(const std::string& repository)
{
    m_repo = repository;
}

void CTaskInfo::setYear(const int year)
{
    m_year = year;
}

void CTaskInfo::setLanguage(const ApiLanguage language)
{
    m_language = language;
}

void CTaskInfo::setOS(const int os)
{
    m_os = os;
}

void CTaskInfo::setInternal(bool bInternal)
{
    m_bInternal = bInternal;
}

std::ostream& operator<<(std::ostream& os, const CTaskInfo& info)
{
    os << "Name: " << info.m_name << std::endl;
    os << "Path: " << info.m_path << std::endl;
    os << "Short description: " << info.m_shortDescription << std::endl;
    os << "Description: " << info.m_description << std::endl;
    os << "Documentation link: " << info.m_docLink << std::endl;
    os << "Icon path: " << info.m_iconPath << std::endl;
    os << "Keywords: " << info.m_keywords << std::endl;
    os << "Authors link: " << info.m_authors << std::endl;
    os << "Article: " << info.m_article << std::endl;
    os << "Journal/conference: " << info.m_journal << std::endl;
    os << "Year: " << std::to_string(info.m_year) << std::endl;
    os << "Version: " << info.m_version << std::endl;
    os << "API version: " << info.m_ikomiaVersion << std::endl;
    os << "Created: " << info.m_createdDate << std::endl;
    os << "Modified: " << info.m_modifiedDate << std::endl;
    os << "License: " << info.m_license << std::endl;
    os << "Repository: " << info.m_repo << std::endl;
    std::string language = info.m_language == ApiLanguage::CPP ? "C++" : "Python";
    os << "Language: " << language << std::endl;
    std::string system = info.m_os == OSType::LINUX ? "Linux" : (info.m_os == OSType::WIN ? "Windows" : "MacOS");
    os << "OS: " << system << std::endl;
    return os;
}
