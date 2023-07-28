#include "Resource.hpp"
#include <exception>

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

ResourceManager::ResourceManager()
{
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

EmbedResource::EmbedResource()
{
}
