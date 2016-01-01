require 'pry'
LIBS = FileList.new( %w( SDL2_test SDL2 GLEW GL GLU pthread BulletSoftBody BulletDynamics BulletCollision LinearMath )).pathmap("-l%X")
INCLUDE = FileList.new( %w( headers /usr/include/bullet )).pathmap("-I%X")
PARAMS = FileList.new %w( -Wall -Werror -std=c++14 -stdlib=libc++ )
INPUT_FILES = FileList.new( "src/*.cpp")
OBJ_FILES = INPUT_FILES.pathmap("%{^src/,obj/}X.o")


task default: :build
desc "compiles all .cpp files in /src"
task compile: OBJ_FILES
desc "creates debug executable"
task build: [:debug_params,:compile, :link]
desc "creates release executable"
task release: [:release_params,:compile, :link]
desc "links all .o files in /obj"
task :link do
	#`g++ #{INCLUDE} #{PARAMS} -o20pinbowling ./obj/* #{LIBS}`
	#`g++ #{INCLUDE} #{PARAMS} -o20pinbowling-#{$stage} #{OBJ_FILES} #{LIBS}`
	puts "Linking #{$stage}"
	`clang++-3.6 #{INCLUDE} #{PARAMS} -o20pinbowling-#{$stage} #{OBJ_FILES} #{LIBS}`
end

task :debug_params do
	$stage = "debug"
	#PARAMS << FileList.new(%w( -gstabs ))
	PARAMS << FileList.new(%w( -g ))
end
task :release_params do
	$stage = "release"
	PARAMS << FileList.new(%w( -O3 ))
end
rule ".o" => ->(t){cpp_source(t)} do |f|
	#`g++ #{INCLUDE} #{PARAMS} -c -o#{f.name} #{f.source}`
	puts "Compiling: #{f.name} with #{f.source}"
	`clang++-3.6 #{INCLUDE} #{PARAMS} -c -o#{f.name} #{f.source}`
end

desc "cleans .obj files"
task :clean_obj do
	`rm -rf obj/*.o`
end
def cpp_source(file)
	INPUT_FILES.detect{|f| f.pathmap("%n") == file.pathmap("%n")}
end


