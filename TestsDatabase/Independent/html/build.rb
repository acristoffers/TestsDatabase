#!/usr/bin/env ruby

require 'ftools'
require 'digest/md5'

$list = ''

def full_path(parent, obj)
  parent.path + '/' + obj
end

def recurse(dir)
  dir = Dir.new(dir) if dir.class != Dir
  dir.entries.sort.each do |sd|
    next if sd == '..' or sd == '.'

    if File.directory? full_path(dir, sd)
      recurse Dir.new( full_path(dir, sd) )
    else
      if sd.end_with? 'js'
        `uglifyjs2 #{full_path(dir, sd)} -c -m -o #{full_path(dir, sd)}`
      else
        if sd.end_with? 'css' or sd.end_with? 'htm' or sd.end_with? 'html'
          css_comment = /\/\*[^\*]+\*\//
          htm_comment = /<!--[^>]+-->/
          
          str = File.read(full_path(dir, sd))
          
          str.gsub! css_comment, ''
          str.gsub! htm_comment, ''
          str.gsub! "\n", ''
          str.gsub! "\t", ' '
          str.gsub! /[ ]+/, ' '
          
          if sd.end_with? 'css'
            str.gsub! /[ ]*;[ ]*/, ';'
            str.gsub! /[ ]*:[ ]*/, ':'
            str.gsub! /[ ]*,[ ]*/, ','
            str.gsub! /[ ]*\)[ ]*/, ')'
            str.gsub! /[ ]*\([ ]*/, '('
            str.gsub! /[ ]*\{[ ]*/, '{'
            str.gsub! /[ ]*\}[ ]*/, '}'
            str.gsub! ';}', '}'
          end
          
          File.open full_path(dir, sd), 'w' do |f|
            f.write str
          end
        end
      end
      $list += full_path(dir, sd) + '=' + Digest::MD5.hexdigest(File.read(full_path(dir, sd))) + "\n" if sd.end_with? 'png' or sd.end_with? 'jpg' or sd.end_with? 'jpeg' or sd.end_with? 'gif' or sd.end_with? 'js' or sd.end_with? 'css' or sd.end_with? 'htm' or sd.end_with? 'html'
    end
  end
end

recurse '.'

File.open('blob.txt', 'w') do |file|
  file.write($list)
end

`./compressor blob.txt > blob`

File.delete 'blob.txt'
File.delete 'compressor'
File.delete 'build.rb'