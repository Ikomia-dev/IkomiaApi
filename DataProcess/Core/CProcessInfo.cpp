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

#include "CProcessInfo.h"
#include "UtilsTools.hpp"

CProcessInfo::CProcessInfo()
{
    m_ikomiaVersion = Utils::IkomiaApp::getCurrentVersionNumber().toStdString();
}

std::string CProcessInfo::getName() const
{
    return m_name;
}

std::string CProcessInfo::getPath() const
{
    return m_path;
}

std::string CProcessInfo::getShortDescription() const
{
    return m_shortDescription;
}

std::string CProcessInfo::getDescription() const
{
    return m_description;
}

std::string CProcessInfo::getDocumentationLink() const
{
    return m_docLink;
}

std::string CProcessInfo::getIconPath() const
{
    return m_iconPath;
}

std::string CProcessInfo::getKeywords() const
{
    return m_keywords;
}

std::string CProcessInfo::getAuthors() const
{
    return m_authors;
}

std::string CProcessInfo::getArticle() const
{
    return m_article;
}

std::string CProcessInfo::getJournal() const
{
    return m_journal;
}

std::string CProcessInfo::getVersion() const
{
    return m_version;
}

std::string CProcessInfo::getIkomiaVersion() const
{
    return m_ikomiaVersion;
}

std::string CProcessInfo::getCreatedDate() const
{
    return m_createdDate;
}

std::string CProcessInfo::getModifiedDate() const
{
    return m_modifiedDate;
}

std::string CProcessInfo::getLicense() const
{
    return m_license;
}

std::string CProcessInfo::getRepository() const
{
    return m_repo;
}

int CProcessInfo::getYear() const
{
    return m_year;
}

int CProcessInfo::getLanguage() const
{
    return m_language;
}

int CProcessInfo::getOS() const
{
    return m_os;
}

bool CProcessInfo::isInternal() const
{
    return m_bInternal;
}

void CProcessInfo::setName(const std::string &name)
{
    m_name = name;
}

void CProcessInfo::setPath(const std::string &path)
{
    m_path = path;
}

void CProcessInfo::setShortDescription(const std::string &description)
{
    m_shortDescription = description;
}

void CProcessInfo::setDescription(const std::string &description)
{
    m_description = description;
}

void CProcessInfo::setDocumentationLink(const std::string &link)
{
    m_docLink = link;
}

void CProcessInfo::setIconPath(const std::string &path)
{
    m_iconPath = path;
}

void CProcessInfo::setKeywords(const std::string &keywords)
{
    m_keywords = keywords;
}

void CProcessInfo::setAuthors(const std::string &authors)
{
    m_authors = authors;
}

void CProcessInfo::setArticle(const std::string &article)
{
    m_article = article;
}

void CProcessInfo::setJournal(const std::string &journal)
{
    m_journal = journal;
}

void CProcessInfo::setVersion(const std::string &version)
{
    m_version = version;
}

void CProcessInfo::setCreatedDate(const std::string &date)
{
    m_createdDate = date;
}

void CProcessInfo::setModifiedDate(const std::string &date)
{
    m_modifiedDate = date;
}

void CProcessInfo::setLicense(const std::string& license)
{
    m_license = license;
}

void CProcessInfo::setRepository(const std::string& repository)
{
    m_repo = repository;
}

void CProcessInfo::setYear(const int year)
{
    m_year = year;
}

void CProcessInfo::setLanguage(const int language)
{
    m_language = language;
}

void CProcessInfo::setOS(const int os)
{
    m_os = os;
}

void CProcessInfo::setInternal(bool bInternal)
{
    m_bInternal = bInternal;
}
