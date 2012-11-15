#!/usr/bin/env ruby

require 'ftools'
require 'digest/md5'

$list = ''

def full_path(parent, obj)
  #File.expand_path
  parent.path + '/' + obj
end

def recurse(dir)
  dir = Dir.new(dir) if dir.class != Dir
  dir.entries.sort.each do |sd|
    next if sd == '..' or sd == '.'

    if File.directory? full_path(dir, sd)
      recurse Dir.new( full_path(dir, sd) )
    else
      $list += full_path(dir, sd) + '=' + Digest::MD5.hexdigest(File.read(full_path(dir, sd))) + "\n" if sd.end_with? 'png' or sd.end_with? 'jpg' or sd.end_with? 'jpeg' or sd.end_with? 'gif' or sd.end_with? 'js' or sd.end_with? 'css' or sd.end_with? 'htm' or sd.end_with? 'html'
    end
  end
end

recurse '.'

$list.gsub! 'MD5 (', ''
$list.gsub! ') = ', '='
File.open('blob.txt', 'w') do |file|
  file.write($list)
end