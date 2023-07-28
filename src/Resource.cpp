#include "Resource.hpp"
#include <array>
#include <codecvt>
#include <exception>
#include <iostream>

const char16_t       ResourceLocation::DOMAIN_SEPARATOR = u':';
const std::u16string ResourceLocation::DEFAULT_DOMAIN   = u"default";
const std::u16string ResourceLocation::EMBED_DOMAIN     = u"embed";

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

static const std::u16string DEFAULT_ASSETS_DIR = u"assets";

extern "C" const uint8_t _embed_indexAssets[];
extern "C" const uint8_t _embed_blockAssets[];

ResourceManager::ResourceManager()
{
    EmbedResource(_embed_indexAssets, _embed_blockAssets);
}

std::ifstream ResourceManager::get(const ResourceLocation& loc)
{
    auto it = providers.find(loc.domain);
    if (it != providers.end())
    {
        return it->second->get(loc);
    }

    return {};
}

std::ifstream EmbedProvider::get(const ResourceLocation& loc)
{
    return {};
}

FileProvider::FileProvider(const std::u16string root)
    : root(root)
{
}

std::ifstream FileProvider::get(const ResourceLocation& loc)
{
    auto stream = std::ifstream(root / loc.path, std::ios_base::binary);
    return stream;
}

EmbedResource::EmbedResource(const Index_t index, const uint8_t* block)
    : index(index)
    , block(block)
{
}

EmbedResource::EmbedResource(const uint8_t* index, const uint8_t* block)
    : EmbedResource(_make_index(index), block)
{
}

EmbedResource::Index_t EmbedResource::_make_index(const uint8_t* index)
{
    EmbedResource::Index_t _index;

    size_t offset = 0;
    auto   count  = _get<size_t>(index, offset);
    for (size_t i = 0; i < count; i++)
    {
        auto e_path   = _get_path(index, offset);
        auto e_size   = _get<size_t>(index, offset);
        auto e_offset = _get<size_t>(index, offset);

        _index.emplace(e_path, std::pair(e_offset, e_size));
        std::wcout << "Path: " << (wchar_t*)e_path.c_str() << " Offset: " << e_offset << " Size: " << e_size
                   << std::endl;
    }

    return _index;
}

std::u16string EmbedResource::_get_path(const uint8_t* index, size_t& offset)
{
    // String size
    auto str_s = _get<size_t>(index, offset);

    std::u16string str((std::u16string::value_type*)&index[offset], str_s);
    offset += str_s;

    return str;
}