#include "Resource.hpp"
#include <array>
#include <codecvt>
#include <exception>
#include <iostream>

const char16_t ResourceLocation::DOMAIN_SEPARATOR = u':';

ResourceLocation::ResourceLocation(const std::u16string key)
    : ResourceLocation(getDomain(key), getPath(key))
{
}

ResourceLocation::ResourceLocation(const std::u16string domain, const std::u16string path) noexcept
    : domain(domain)
    , path(path)
{
}

bool ResourceLocation::operator==(const ResourceLocation& b) const noexcept
{
    return this->domain == b.domain && this->path == b.path;
}

bool ResourceLocation::operator<(const ResourceLocation& b) const noexcept
{
    return this->domain < b.domain || (this->domain == b.domain && this->path < b.path);
}

std::u16string ResourceLocation::getDomain(const std::u16string key)
{
    auto i = key.find_first_of(DOMAIN_SEPARATOR);
    if (i < 0) throw std::invalid_argument("missing domain");
    auto domain = key.substr(0, i);
    return domain;
}

std::u16string ResourceLocation::getPath(const std::u16string key)
{
    auto i = key.find_first_of(DOMAIN_SEPARATOR);
    if (i < 0) throw std::invalid_argument("missing domain");
    auto path = key.substr(i + 1);
    return path;
}

const std::u16string ResourceManager::DEFAULT_ASSETS_DIR = u"assets";
const std::u16string ResourceManager::DEFAULT_DOMAIN     = u"default";
const std::u16string ResourceManager::EMBED_DOMAIN       = u"embed";

extern "C" const uint8_t _embed_indexAssets[];
extern "C" const uint8_t _embed_blockAssets[];

ResourceManager::ResourceManager()
{
    auto embed_res = std::make_unique<EmbedResource>(_embed_indexAssets, _embed_blockAssets);
    providers.emplace(EMBED_DOMAIN, std::move(embed_res));

    auto default_res = std::make_unique<FileResource>(DEFAULT_ASSETS_DIR);
    providers.emplace(DEFAULT_DOMAIN, std::move(default_res));
}

ResourceStream ResourceManager::get(const ResourceLocation& loc)
{
    auto it = providers.find(loc.domain);
    if (it != providers.end())
    {
        return it->second->get(loc);
    }

    return {};
}

FileResource::FileResource(const std::u16string root)
    : root(root)
{
}

ResourceStream FileResource::get(const ResourceLocation& loc)
{
    auto stream = std::make_unique<std::ifstream>(root / loc.path, std::ios_base::binary);
    if (stream->fail()) return {};
    return stream;
}

EmbedResource::EmbedResource(const Index index, const uint8_t* block)
    : index(index)
    , block(block)
{
}

EmbedResource::EmbedResource(const uint8_t* index, const uint8_t* block)
    : EmbedResource(_make_index(index), block)
{
}

ResourceStream EmbedResource::get(const ResourceLocation& loc)
{
    auto it = index.find(loc.path);
    if (it != index.end())
    {
        auto offset = it->second.first;
        auto size   = it->second.second;

        auto stream = std::make_unique<imstream>(&block[offset], size);
        return stream;
    }
    return {};
}

EmbedResource::Index EmbedResource::_make_index(const uint8_t* index)
{
    EmbedResource::Index _index;

    size_t offset = 0;
    auto   count  = _get<size_t>(index, offset);
    for (size_t i = 0; i < count; i++)
    {
        auto e_path   = _get_path(index, offset);
        auto e_size   = _get<size_t>(index, offset);
        auto e_offset = _get<size_t>(index, offset);
        auto e_hash   = std::hash<std::u16string>{}(e_path);

        _index.emplace(e_path, std::pair(e_offset, e_size));
        std::wcout << "Path: " << (wchar_t*)e_path.c_str() << " Hash: " << e_hash << " Offset: " << e_offset
                   << " Size: " << e_size << std::endl;
    }

    return _index;
}

std::u16string EmbedResource::_get_path(const uint8_t* index, size_t& offset)
{
    // String size
    auto str_s = _get<size_t>(index, offset);

    std::u16string str((std::u16string::value_type*)&index[offset], str_s);
    offset += str_s * sizeof(std::u16string::value_type);

    return str;
}