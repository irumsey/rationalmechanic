#include "Material.h"
#include "Context.h"
#include "Resources.h"
#include "Types.h"
#include <lucid/gal/Program.h>
#include <lucid/gal/Texture2D.h>
#include <lucid/gal/Unordered2D.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/Types.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Reader.h>
#include <lucid/core/Logger.h>
#include <algorithm>

LUCID_ANONYMOUS_BEGIN

typedef LUCID_GAL::Parameter parameter_t;
typedef LUCID_GAL::Program program_t;

typedef LUCID_GAL::Unordered2D unordered2d_t;
typedef std::shared_ptr<unordered2d_t> shared_unordered2d_t;

typedef LUCID_GAL::Texture2D texture2d_t;
typedef std::shared_ptr<texture2d_t> shared_texture2d_t;

typedef LUCID_GAL::RenderTarget2D target2d_t;
typedef std::shared_ptr<target2d_t> shared_target2d_t;

typedef LUCID_GIGL::Primitive primitive_t;

typedef LUCID_GIGL::Context context_t;

typedef void (*apply_func_t)(context_t const &, program_t const *, parameter_t const *, primitive_t const &);

extern apply_func_t const apply_jump[primitive_t::TYPE_COUNT];

LUCID_ANONYMOUS_END

LUCID_GIGL_BEGIN

///	Applicator
///	
///
struct Applicator
{
	Context const &context;
	program_t const *program = nullptr;

	Applicator(Context const &context, program_t const *program)
		: context(context)
		, program(program)
	{
	}

	inline void operator()(std::pair<std::string,Material::Attribute> const &entry) const
	{
		Material::Attribute const &attribute = entry.second;

		Primitive::Info const &info = attribute.value.info();
		apply_jump[info.tid](context, program, attribute.parameter, entry.second.value);
	}
};

///
///
///

Material::Material(std::string const &path)
{
	initialize(::lucid::core::FileReader(path));
}

Material::Material(core::Reader &reader)
{
	initialize(reader);
}

Material::~Material()
{
	shutdown();
}
		
void Material::begin(Context const &context) const
{
	gal::Pipeline &pipeline = gal::Pipeline::instance();

	pipeline.updateTargets();
	std::for_each(_attributes.begin(), _attributes.end(), Applicator(context, _program.get()));
	pipeline.beginProgram(_program.get());
}

void Material::end()
{
	gal::Pipeline &pipeline = gal::Pipeline::instance();
	pipeline.endProgram(_program.get());
}

Material *Material::create(std::string const &path)
{
	return new Material(path);
}

Material *Material::create(core::Reader &reader)
{
	return new Material(reader);
}

void Material::initialize(::lucid::core::Reader &reader)
{
	if (reader.read<bool>())
	{
		_program = Resources::get<program_t>(reader.read<std::string>());
	}
	else
	{
		_program.reset(::lucid::gal::Program::create(reader));
	}

	int32_t count = reader.read<int32_t>();
	for (int32_t i = 0; i < count; ++i)
	{
		std::string name = reader.read<std::string>();
		_attributes[name] = Attribute(_program->lookup(name), Primitive(reader));
	}
}

void Material::shutdown()
{
	///	NOP
}

LUCID_GIGL_END

LUCID_ANONYMOUS_BEGIN

template<class T> void apply(context_t const &, program_t const *program, parameter_t const *parameter, primitive_t const &value)
{
	program->set(parameter, value.as<T>());
}

template<> void apply<std::string>(context_t const &context, program_t const *program, parameter_t const *parameter, primitive_t const &value)
{
	std::string const &query = value.as<std::string>();

	primitive_t const &result = context[query];
	primitive_t::Info const &info = result.info();

	apply_jump[info.tid](context, program, parameter, result);
}

template<> void apply<shared_unordered2d_t>(context_t const &context, program_t const *program, parameter_t const *parameter, primitive_t const &value)
{
	shared_unordered2d_t texture = value.as<shared_unordered2d_t>();
	program->set(parameter, texture.get());
}

template<> void apply<shared_texture2d_t>(context_t const &context, program_t const *program, parameter_t const *parameter, primitive_t const &value)
{
	shared_texture2d_t texture = value.as<shared_texture2d_t>();
	program->set(parameter, texture.get());
}

template<> void apply<shared_target2d_t>(context_t const &context, program_t const *program, parameter_t const *parameter, primitive_t const &value)
{
	shared_target2d_t target = value.as<shared_target2d_t>();
	program->set(parameter, target.get());
}

template<> void apply<primitive_t::UNKNOWN>(context_t const &, program_t const *, parameter_t const *, primitive_t const &)
{
	LUCID_THROW("attempt to set unknown parameter type");
}

template<> void apply<primitive_t::UNDEFINED>(context_t const &, program_t const *, parameter_t const *, primitive_t const &)
{
	LUCID_THROW("attempt to set undefined parameter");
}

apply_func_t const apply_jump[primitive_t::TYPE_COUNT] =
{
	apply<primitive_t::UNKNOWN>,
	apply<primitive_t::UNDEFINED>,
	apply<bool>,
	apply<int32_t>,
	apply<LUCID_GIGL::Scalar>,
	apply<LUCID_GIGL::Color>,
	apply<LUCID_GIGL::Vector2>,
	apply<LUCID_GIGL::Vector3>,
	apply<LUCID_GIGL::Vector4>,
	apply<LUCID_GIGL::Quaternion>,
	apply<LUCID_GIGL::Matrix2x2>,
	apply<LUCID_GIGL::Matrix3x3>,
	apply<LUCID_GIGL::Matrix4x4>,
	apply<shared_texture2d_t>,
	apply<shared_target2d_t>,
	apply<std::string>,
	apply<shared_unordered2d_t>,
};

LUCID_ANONYMOUS_END