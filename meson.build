project('amashell', 'c')
subdir('src')
amash = executable('amash', amash_sources, link_args: '-lreadline', install: true)
run_target('run', command:'scripts/run_shell.sh', depends:amash)
