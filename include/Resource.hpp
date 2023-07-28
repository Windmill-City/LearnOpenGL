#pragma once
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

struct ResourceLocation
{
    const static char16_t       DOMAIN_SEPARATOR;
    const static std::u16string DEFAULT_DOMAIN;
    const static std::u16string EMBED_DOMAIN;

    /**
     * @brief Resource n
     * eg: default:shader/VertexShader.txt, the domain is 'default'
     */
    const std::u16string domain;
    /**
     * @brief Resource path
     * eg: default:shader/VertexShader.txt, the path is 'shader/VertexShader.txt'
     */
    const std::u16string path;

    /**
     * @brief Construct a new Resource Location object
     *
     * @param key resource key, eg: default:shader/VertexShader.txt
     */
    ResourceLocation(const std::u16string key);

    /**
     * @brief Construct a new Resource Location object
     *
     * @param domain resource domain
     * @param path resource path
     */
    ResourceLocation(const std::u16string domain, const std::u16string path) noexcept;

    bool operator==(const ResourceLocation& b) const noexcept;
    bool operator<(const ResourceLocation& b) const noexcept;

    /**
     * @brief Get domain of the resource key
     *
     * @return std::u16string_view domain
     */
    static std::u16string getDomain(const std::u16string key);

    /**
     * @brief Get the path of the resource key
     *
     * @return std::u16string_view path
     */
    static std::u16string getPath(const std::u16string key);
};

struct ResourceProvider
{
    /**
     * @brief Get file stream by ResourceLocation
     *
     * @param loc resource location
     * @return std::istream file stream or empty
     */
    virtual std::ifstream get(const ResourceLocation& loc) = 0;

    ResourceProvider() = default;

    // Global object, should not copy or move
    ResourceProvider(const ResourceProvider&)             = delete;
    ResourceProvider& operator=(const ResourceProvider&)  = delete;
    ResourceProvider(const ResourceProvider&&)            = delete;
    ResourceProvider& operator=(const ResourceProvider&&) = delete;
};

struct ResourceManager : public ResourceProvider
{
  public:
    const static std::u16string DEFAULT_ASSETS_DIR;

    /**
     * @brief Domain providers
     * Key: Domain, Value: Provider
     */
    const std::unordered_map<std::u16string, std::unique_ptr<ResourceProvider>> providers;

    ResourceManager();
    virtual std::ifstream get(const ResourceLocation& loc) override;
};

struct EmbedResource
{
    /**
     * @brief Resource index
     * Key: Path, Val:Pair<Offset,Size>
     */
    using Index_t = std::unordered_map<std::u16string, const std::pair<size_t, size_t>>;

    /**
     * @brief Resource domain
     *
     */
    const std::u16string domain;
    const uint8_t*       block;
    const Index_t        index;

    EmbedResource(const Index_t index, const uint8_t* block);
    EmbedResource(const uint8_t* index, const uint8_t* block);

    static Index_t _make_index(const uint8_t* index);

    template <class T>
    static T _get(const uint8_t* index, size_t& offset)
    {
        auto val = *(T*)&index[offset];
        offset += sizeof(T);
        return val;
    }

    static std::u16string _get_str(const uint8_t* index, size_t& offset);
};

struct EmbedProvider : public ResourceProvider
{
    const std::unique_ptr<EmbedResource> res;

    EmbedProvider(const std::unique_ptr<const EmbedResource> res);
    virtual std::ifstream get(const ResourceLocation& loc) override;
};

struct FileProvider : public ResourceProvider
{
    const std::filesystem::path root;

    FileProvider(const std::u16string root);
    virtual std::ifstream get(const ResourceLocation& loc) override;
};