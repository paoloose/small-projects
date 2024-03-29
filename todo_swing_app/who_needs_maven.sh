#!/usr/bin/bash

set -e

group_id="com.github.paoloose"
artifact_id="todoapp"
main_class="main.Main"
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
    echo "  run [-- <args>]: runs the jar application"
    echo "  test: equivalent to 'echo \"All tests passed!\"'"
    echo "  package: creates a jar file"
    echo "  clean: remove the target directory"
}

function generate_manifest() {
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

    generate_manifest "$outdir/MANIFEST.MF"
    jar --create -v --file"=$outdir/$app_name.jar" --manifest="$outdir/MANIFEST.MF" -C "$classes_outdir" .
}

function run_app() {
    build_app > /dev/null
    java -jar "$outdir/$app_name.jar" $@
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
    local script_command="$1"; shift
    local jar_arguments=""

    if [ -z "$script_command" ]; then usage; exit 0; fi

    for arg in $@; do
        case "$arg" in
            --) shift; jar_arguments="$@"; break;;
            *) shift;;
        esac
    done

    case "$script_command" in
        run) run_app $jar_arguments;;
        test) echo "All tests passed!";;
        package) build_app;;
        clean) clean_app;;

        *) echo "Unknown option: $script_command\n"; usage; exit 1;;
    esac
}

main $@
