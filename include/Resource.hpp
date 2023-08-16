#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

struct ResourceLocation
{
    const static wchar_t DOMAIN_SEPARATOR;

    /**
     * @brief Resource n
     * eg: default:shader/VertexShader.txt, the domain is 'default'
     */
    const std::wstring domain;
    /**
     * @brief Resource path
     * eg: default:shader/VertexShader.txt, the path is 'shader/VertexShader.txt'
     */
    const std::wstring path;

    /**
     * @brief Construct a new Resource Location object
     *
     * @param key resource key, eg: default:shader/VertexShader.txt
     */
    ResourceLocation(const std::wstring::value_type* key);
    /**
     * @brief Construct a new Resource Location object
     *
     * @param key resource key, eg: default:shader/VertexShader.txt
     */
    ResourceLocation(const std::wstring key);

    /**
     * @brief Construct a new Resource Location object
     *
     * @param domain resource domain
     * @param path resource path
     */
    ResourceLocation(const std::wstring domain, const std::wstring path) noexcept;

    bool operator==(const ResourceLocation& b) const noexcept;
    bool operator<(const ResourceLocation& b) const noexcept;

    /**
     * @brief Get domain of the resource key
     *
     * @return std::wstring_view domain
     */
    static std::wstring getDomain(const std::wstring key);

    /**
     * @brief Get the path of the resource key
     *
     * @return std::wstring_view path
     */
    static std::wstring getPath(const std::wstring key);
};

using ResourceStream = std::unique_ptr<std::istream>;

struct ResourceProvider
{
    /**
     * @brief Get file stream by ResourceLocation
     *
     * @param loc resource location
     * @return ResourceStream a valid stream or nullptr
     */
    virtual ResourceStream get(const ResourceLocation& loc) = 0;

    ResourceProvider() = default;

    // Global object, should not copy or move
    ResourceProvider(const ResourceProvider&)             = delete;
    ResourceProvider& operator=(const ResourceProvider&)  = delete;
    ResourceProvider(const ResourceProvider&&)            = delete;
    ResourceProvider& operator=(const ResourceProvider&&) = delete;
};

struct ResourceManager : public ResourceProvider
{
    using ProviderHolder = std::unordered_map<std::wstring, std::unique_ptr<ResourceProvider>>;

    const static std::wstring DEFAULT_ASSETS_DIR;
    const static std::wstring DEFAULT_DOMAIN;
    const static std::wstring EMBED_DOMAIN;

    /**
     * @brief Domain providers
     * Key: Domain, Value: Provider
     */
    ProviderHolder providers;

    ResourceManager();
    virtual ResourceStream get(const ResourceLocation& loc) override;
};

struct EmbedResource : public ResourceProvider
{
    struct membuf : std::streambuf
    {
        membuf(const uint8_t* base, size_t size);
        pos_type seekoff(off_type               _Off,
                         std::ios_base::seekdir _Way,
                         std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;
        pos_type seekpos(pos_type, std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;
    };

    struct imstream
        : virtual membuf
        , std::istream
    {
        imstream(const uint8_t* base, size_t size);
    };

    /**
     * @brief Resource index
     * Key: Path, Val:Pair<Offset,Size>
     */
    using Index = std::unordered_map<std::wstring, const std::pair<size_t, size_t>>;

    /**
     * @brief Resource domain
     *
     */
    const std::wstring   domain;
    /**
     * @brief Resource block, contains continuously stored files
     *
     */
    const uint8_t*       block;
    const Index          index;

    EmbedResource(const Index index, const uint8_t* block);
    EmbedResource(const uint8_t* index, const uint8_t* block);
    virtual ResourceStream get(const ResourceLocation& loc) override;

    static Index _make_index(const uint8_t* index);

    template <class T, typename = std::enable_if_t<std::is_trivial_v<T>>>
    static T _get(const uint8_t* index, size_t& offset)
    {
        auto val = *(T*)&index[offset];
        offset += sizeof(T);
        return val;
    }

    static std::wstring _get_path(const uint8_t* index, size_t& offset);
};

struct FileResource : public ResourceProvider
{
    const std::filesystem::path root;

    FileResource(const std::wstring root);
    virtual ResourceStream get(const ResourceLocation& loc) override;
};