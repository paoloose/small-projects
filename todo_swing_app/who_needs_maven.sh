#!/usr/bin/bash

set -e

group_id="com.github.paoloose"
artifact_id="todoapp"
main_class="main.TodoApp"
app_name="todoapp" # jar output name

src_prefix="src/main/java"
test_prefix="src/test/java"
outdir="target"

# replace the syntax <tld>.<domain>.<artifact> to <tld>/<domain>/<artifact>
#   where <tld>: top level domain
#   see: https://maven.apache.org/guides/mini/guide-naming-conventions.html
project_dir=$(echo "$group_id.$artifact_id" | tr "." "/")

function usage() {
    echo "usage: $0 <command>"
    echo
    echo "commands:"
    echo "  run: run the application"
    echo "  test: equivalent to 'echo \"All tests passed!\"'"
    echo "  package: creates a jar file"
    echo "  clean: remove the target directory"
}

function create_manifest() {
    local destination="$1"
    local manifest=''
    manifest+="Manifest-Version: 1.0\n"
    manifest+="Created-By: 17.0.8-ea (Debian)\n"
    manifest+="Main-Class: $group_id.$artifact_id.$main_class\n"

    printf "%b" "$manifest" > $destination
}

function build_app() {
    local java_classes=$(find $src_prefix/$project_dir -name '*.java')

    if [ -z "$java_classes" ]; then
        echo "No java src files found in $src_prefix/$project"
        echo "Maybe you should start to write some code... BTW, have you heard about TDD?\n"
        echo "I mean, don't get me wrong, I'm not judging you or anything, but it's"
        echo "just that I've seen a lot of people getting stuck because they didn't"
        echo "write tests first, and then they had to rewrite everything, and it"
        echo "wasn't pretty, you know?"
        exit 1
    fi

    local classes_outdir=$outdir/classes
    mkdir -p "$classes_outdir"

    local compilation_output=$(javac -d "$classes_outdir" $java_classes 2>&1)

    if [ -n "$compilation_output" ]; then
        echo -e "Compilation failed! 😢\n" 1>&2
        echo "$compilation_output" 1>&2
        exit 1
    fi

    cp -r "src/main/resources" "$classes_outdir" 2> /dev/null || :

    pushd $classes_outdir
        local class_objects=$(find . -name '*.class')
    popd

    echo
    echo $class_objects | tr " " "\n"
    echo
    echo $classes_outdir
    echo $(pwd)
    echo

    create_manifest "$outdir/MANIFEST.MF"
    jar --create -v --file"=$outdir/$app_name.jar" --manifest="$outdir/MANIFEST.MF" -C "$classes_outdir" $class_objects
}

function run_app() {
    build_app > /dev/null
    java -jar "$outdir/$app_name.jar"
}

function clean_app() {
    # the french way
    if [ -d "$outdir" ]; then
        echo "Removing target directory... 🐢"
        rm -fr "$outdir"
    else
        echo "Nothing to clean, you are good to go! 🐢"
    fi
}

main() {
    local script_command="$1"

    if [ -z "$script_command" ]; then usage; exit 0; fi

    case "$script_command" in
        run) run_app;;
        test) echo "All tests passed!";;
        package) build_app;;
        clean) clean_app;;

        *) echo "Unknown option: $script_command\n"; usage; exit 1;;
    esac
}

main $@
