require 'pry'
LIBS = FileList.new( %w( SDL2_test SDL2 GL GLU pthread BulletSoftBody BulletDynamics BulletCollision LinearMath )).pathmap("-l%X")
INCLUDE = FileList.new( %w( headers /usr/include/bullet )).pathmap("-I%X")
PARAMS = FileList.new %w( -Wall -Werror )
INPUT_FILES = FileList.new( "src/*")
OBJ_FILES = INPUT_FILES.pathmap("%{^src/,obj/}X.o")


task default: :build
task compile: OBJ_FILES
task build: [:debug_params,:compile, :link]
task release: [:release_params,:compile, :link]
task :link do
	#`g++ #{INCLUDE} #{PARAMS} -o20pinbowling ./obj/* #{LIBS}`
	`g++ #{INCLUDE} #{PARAMS} -o20pinbowling #{OBJ_FILES} #{LIBS}`
end

task :debug_params do
	PARAMS << FileList.new(%w( -gstabs ))
end
task :release_params do
	PARAMS << FileList.new(%w( -O3 ))
end
rule ".o" => ->(t){cpp_source(t)} do |f|
	`g++ #{INCLUDE} #{PARAMS} -c -o#{f.name} #{f.source}`
end
def cpp_source(file)
	INPUT_FILES.detect{|f| f.pathmap("%n") == file.pathmap("%n")}
end


