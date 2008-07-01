/***************************************************************************
                        pluginloader.cpp  -  description
                             -------------------
    begin                : Sun Oct 7 2007
    copyright            : (C) 2007 by Dominik Seichter
    email                : domseichter@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pluginloader.h"

#include "plugin.h"

#include "datetimeplugin.h"
#include "fileplugin.h"
#include "permissionsplugin.h"
#include "scriptplugin.h"
#include "systemplugin.h"
#include "taglibplugin.h"
#include "translitplugin.h"

#include <kservice.h>

PluginLoader* PluginLoader::s_instance = NULL;

PluginLoader* PluginLoader::Instance()
{
    if( !s_instance ) 
        s_instance = new PluginLoader();

    return s_instance;
}


PluginLoader::PluginLoader()
{
    this->load();
}

PluginLoader::~PluginLoader()
{
    this->clear();
}

Plugin* PluginLoader::findPlugin( const QString & token )
{
    QString lower = token.toLower();

    // first search in the cache
    Plugin* p = m_tokenCache[lower];
    if( p ) 
        return p;


    // now search in all tokens
    QMap<QString,Plugin*>::const_iterator it = m_tokenMap.begin();
    while( it != m_tokenMap.end() )
    {
        if( QRegExp( it.key() ).exactMatch( lower ) )
        {
            // we found a plugin
            // put the token into the cache for quick access
            p = it.value();
            m_tokenCache[lower] = p;
            return p;
        }

        ++it;
    }

    // add typos to the cache, too:
    // So that we find immediately that this key is not supported.
    m_tokenCache.insert( lower, NULL );
    return NULL;
}

void PluginLoader::clear()
{
    QList<Plugin*>::iterator it = m_plugins.begin();

    while( it != m_plugins.end() )
    {
        delete *it;

        ++it;
    }

    m_plugins.clear();
    m_tokenMap.clear();
    m_tokenCache.clear();
}

void PluginLoader::load()
{
    m_plugins.append( new DateTimePlugin() );
    m_plugins.append( new PermissionsPlugin() );
    m_plugins.append( new ScriptPlugin() );
    m_plugins.append( new SystemPlugin() );
    m_plugins.append( new TagLibPlugin() );
    m_plugins.append( new TranslitPlugin() );
    //this->loadFilePlugins();


    // Fill the token map
    QList<Plugin*>::iterator it = m_plugins.begin();
    while( it != m_plugins.end() )
    {
        if( ((*it)->type() & ePluginType_Token) ) 
        {
            const QStringList & tokens = (*it)->supportedTokens(); 
            QStringList::const_iterator itList = tokens.begin();
            while( itList != tokens.end() )
            {
                m_tokenMap.insert( (*itList).toLower(), *it );
                ++itList;
            }
        }

        ++it;
    }
}

