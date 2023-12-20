#include <graphics/ModelResource.h>
#include <graphics/ShaderHolder.h>

#include <g3d/aglShaderUtilG3D.h>
//#include <gfx/seadGraphics.h>
#include <math/rio_Math.h>

ModelResource::ModelResource()
    : mResFile(nullptr)
    , mModelShaderArchive()
    , _1c(0)
{
}

ModelResource::~ModelResource()
{
    if (mResFile)
        destroy();
}

void ModelResource::destroy()
{
    if (mModelShaderArchive.isBufferReady())
    {
        for (s32 i = 0; i < mResFile->GetModelCount(); i++)
        {
            if (mModelShaderArchive[i].obj)
            {
                mModelShaderArchive[i].obj->destroy();
                delete mModelShaderArchive[i].obj;

                if (mModelShaderArchive[i].res_binary_archive)
                    rio::MemUtil::free(mModelShaderArchive[i].res_binary_archive);

                if (mModelShaderArchive[i].res_archive)
                    rio::MemUtil::free(mModelShaderArchive[i].res_archive);
            }
        }

        mModelShaderArchive.freeBuffer();
    }

    mResFile->Cleanup();
    rio::MemUtil::free(mResFile);
    mResFile = nullptr;
}

void ModelResource::load(
    const SharcArchiveRes* archive, const char* filename, bool force_sharcfb,
    const nw::g3d::res::ResFile* tex_res_file
)
{
    u32 res_file_size = 0;
    const void* p_res_file_const = archive->getFileConst(
        (std::string(filename) + ".bfres").c_str(),
        &res_file_size
    );
    RIO_ASSERT(p_res_file_const && res_file_size);
    void* p_res_file = rio::MemUtil::alloc(res_file_size, 0x2000);
    rio::MemUtil::copy(p_res_file, p_res_file_const, res_file_size);

    mResFile = nw::g3d::res::ResFile::ResCast(p_res_file);

  //sead::Graphics::instance()->lockDrawContext();
    {
        mResFile->Setup();
    }
  //sead::Graphics::instance()->unlockDrawContext();

    if (tex_res_file)
        mResFile->Bind(tex_res_file, 1);

    s32 num_model = mResFile->GetModelCount();
    if (!mModelShaderArchive.tryAllocBuffer(num_model))
        return;

    for (s32 idx_model = 0; idx_model < num_model; idx_model++)
    {
        agl::ShaderProgramArchive* shader_program_archive = new agl::ShaderProgramArchive;

        nw::g3d::res::ResModel* res_model = mResFile->GetModel(idx_model);
        const char* model_name = res_model->GetName();

        bool is_local = false;

        for (s32 idx_material = 0; idx_material < res_model->GetMaterialCount(); idx_material++)
        {
            nw::g3d::res::ResMaterial* res_material = res_model->GetMaterial(idx_material);
            const char* material_name = res_material->GetName();
            const char* shader_archive_name = res_material->GetShaderAssign()->GetShaderArchiveName();

            bool match = (shader_archive_name == nullptr || shader_archive_name[0] == '\0') ||
                         std::strcmp(shader_archive_name, model_name) == 0 ||
                         std::strcmp(shader_archive_name, material_name) == 0; // Why material name?? It literally uses the model name below tho????

            if (match)
            {
                is_local = true;
            }
            else
            {
                const agl::ShaderProgramArchive* g_shader_program_archive = ShaderHolder::instance()->getShaderArchive(shader_archive_name);
                agl::g3d::ShaderUtilG3D::setMatBlockAndShaderParamOffs(mResFile, g_shader_program_archive, "Mat");
            }
        }

        if (!is_local)
        {
            mModelShaderArchive[idx_model].obj = nullptr;
            // They forgot to free "shader_program_archive"...
            delete shader_program_archive;
            shader_program_archive = nullptr;
        }
        else
        {
            agl::ResShaderArchiveData* res_shader_archive = nullptr;
            agl::ResBinaryShaderArchiveData* res_binary_shader_archive = nullptr;

#if RIO_IS_WIN
            if (force_sharcfb)
#endif // RIO_IS_WIN
            {
                u32 file_size = 0;
                const void* const file = archive->getFileConst(
                    (std::string(model_name) + ".sharcfb").c_str(), // <---- Uses model name
                    &file_size
                );

                if (file && file_size)
                {
                    res_binary_shader_archive = static_cast<agl::ResBinaryShaderArchiveData*>(rio::MemUtil::alloc(file_size, agl::cShaderArchiveAlignment));
                    rio::MemUtil::copy(res_binary_shader_archive, file, file_size);
                }
            }
#if RIO_IS_WIN
            else
            {
                u32 file_size = 0;
                const void* const file = archive->getFileConst(
                    (std::string(model_name) + ".sharc").c_str(), // <---- Uses model name
                    &file_size
                );

                if (file && file_size)
                {
                    res_shader_archive = static_cast<agl::ResShaderArchiveData*>(rio::MemUtil::alloc(file_size, 4));
                    rio::MemUtil::copy(res_shader_archive, file, file_size);
                }
            }
#endif // RIO_IS_WIN

            mModelShaderArchive[idx_model].obj = shader_program_archive;

            mModelShaderArchive[idx_model].res_binary_archive = res_binary_shader_archive;
            mModelShaderArchive[idx_model].res_archive = res_shader_archive;

            shader_program_archive->createWithOption(res_binary_shader_archive, res_shader_archive, 0);

          //sead::Graphics::instance()->lockDrawContext();
            {
                shader_program_archive->setUp();
            }
          //sead::Graphics::instance()->unlockDrawContext();

            mModelShaderArchive[idx_model].initialized = true;

            agl::g3d::ShaderUtilG3D::setMatBlockAndShaderParamOffs(mResFile, shader_program_archive, "Mat");
        }
    }
}

agl::ShaderProgramArchive* ModelResource::getModelShaderProgramArchive(s32 idx_model) const
{
    return mModelShaderArchive[idx_model].obj;
}
