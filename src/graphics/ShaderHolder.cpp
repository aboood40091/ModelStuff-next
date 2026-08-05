#include <graphics/ShaderHolder.h>

#include <filedevice/rio_FileDevice.h>
#include <resource/SZSDecompressor.h>

ShaderHolder* ShaderHolder::sInstance = nullptr;

bool ShaderHolder::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = new ShaderHolder();
    return true;
}

void ShaderHolder::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

ShaderHolder::ShaderHolder()
    : mpArchive(nullptr)
{
    mShaderArchiveMap.reserve(32);
}

ShaderHolder::~ShaderHolder()
{
    for (const auto& it : mShaderArchiveMap)
        delete it.second;

    if (mpArchive)
    {
        mArchiveRes.destroy();
        rio::MemUtil::free(mpArchive);
        mpArchive = nullptr;
    }
}

bool ShaderHolder::initialize(const std::string& arc_path)
{
    if (mpArchive)
    {
        mArchiveRes.destroy();
        rio::MemUtil::free(mpArchive);
        mpArchive = nullptr;
    }

    rio::FileDevice::LoadArg arg;
    arg.path = arc_path;
    arg.path += ".szs";
    arg.alignment = 0x2000;

    mpArchive = SZSDecompressor::tryDecomp(arg);
    if (mpArchive == nullptr)
    {
        RIO_LOG("ShaderHolder::initialize(): failed to load \"%s\"\n", arg.path.c_str());
        return false;
    }

    mArchiveRes.prepareArchive(mpArchive);

    bool success = true;

    /* bg_renderer
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("bg_renderer");
        BgRenderer::initializeShader(p_archive);
        pushBackShaderArchive_("bg_renderer", p_archive);
    }*/
    /* dynamic_light_renderer
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("dynamic_light_renderer");
        DynamicLightRenderer::initializeShader(p_archive);
        pushBackShaderArchive_("dynamic_light_renderer", p_archive);
    }*/
    /* light_mask_shader
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("light_mask_shader");
        LightMaskRenderer::initializeShader(p_archive);
        pushBackShaderArchive_("light_mask_shader", p_archive);
    }*/
    // nw4f_cs_shader
    {
        success = createAndPushBackShaderArchive_("nw4f_cs_shader") && success;
    }
    // nw4f_cs_water_shader
    {
        success = createAndPushBackShaderArchive_("nw4f_cs_water_shader") && success;
    }
    // nw4f_dv_shader
    {
        success = createAndPushBackShaderArchive_("nw4f_dv_shader") && success;
    }
    // nw4f_koopa_shader
    {
        success = createAndPushBackShaderArchive_("nw4f_koopa_shader") && success;
    }
    // nw4f_shader
    {
        success = createAndPushBackShaderArchive_("nw4f_shader") && success;
    }
    /* quad_cloud_shader
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("quad_cloud_shader");
        TexQuadMgr::initializeShader(p_archive);
        pushBackShaderArchive_("quad_cloud_shader", p_archive);
    }*/
    /* mii_custom_shader
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("mii_custom_shader");
        Mii::Shader::initializeShader(p_archive);
        pushBackShaderArchive_("mii_custom_shader", p_archive);
    }*/
    /* mii_icon_faceline_shader
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("mii_icon_faceline_shader");
        Mii::IconFaceline::initializeShader(p_archive);
        pushBackShaderArchive_("mii_icon_faceline_shader", p_archive);
    }*/
    /* mosaic_filter
    {
        agl::ShaderProgramArchive* p_archive = createShaderArchive_("mosaic_filter");
        MosaicFilter::initializeShader(p_archive);
        pushBackShaderArchive_("mosaic_filter", p_archive);
    }*/

    return success;
}

agl::ShaderProgramArchive* ShaderHolder::createShaderArchive_(const std::string& filename) const
{
    agl::ResBinaryShaderArchiveData* res_binary_shader_archive = static_cast<agl::ResBinaryShaderArchiveData*>(
        mArchiveRes.getFileMutable((filename + ".sharcfb").c_str())
    );

    if (res_binary_shader_archive == nullptr)
    {
        RIO_LOG("ShaderHolder: \"%s.sharcfb\" not found in archive\n", filename.c_str());
        return nullptr;
    }

    agl::ShaderProgramArchive* p_archive = new agl::ShaderProgramArchive();
    p_archive->createWithOption(res_binary_shader_archive, nullptr, 2);
    return p_archive;
}

void ShaderHolder::pushBackShaderArchive_(const std::string& filename, agl::ShaderProgramArchive* p_archive)
{
  //sead::Graphics::instance()->lockDrawContext();
    {
        p_archive->setUp();
    }
  //sead::Graphics::instance()->unlockDrawContext();

    mShaderArchiveMap[filename] = p_archive;
}

bool ShaderHolder::createAndPushBackShaderArchive_(const std::string& filename)
{
    agl::ShaderProgramArchive* p_archive = createShaderArchive_(filename);
    if (p_archive == nullptr)
        return false;

    pushBackShaderArchive_(filename, p_archive);
    return true;
}

const agl::ShaderProgramArchive* ShaderHolder::getShaderArchive(const std::string& filename) const
{
    const auto& it = mShaderArchiveMap.find(filename);
    if (it == mShaderArchiveMap.end())
        return nullptr;

    return it->second;
}

const agl::ShaderProgramArchive* ShaderHolder::getNw4fShaderArchive() const
{
    return getShaderArchive("nw4f_shader");
}

const agl::ShaderProgram* ShaderHolder::getNw4fBasicShaderProgram() const
{
    const agl::ShaderProgramArchive* p_archive = getNw4fShaderArchive();
    if (p_archive == nullptr)
        return nullptr;

    return p_archive->searchShaderProgram("nw4f_basic_shader");
}
