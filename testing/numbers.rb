count = 0
print "{ "
0.step(7.9999999, 0.08) do |num|
  print "#{(2 ** num).round}, "
  count += 1
end
puts "255 }"
puts count