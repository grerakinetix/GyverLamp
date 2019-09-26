require 'cmath'

lower_range = 20
upper_range = 45
count = 0
print "uint8_t greens = { "
lower_range.step(upper_range, (upper_range - lower_range).to_f / 100) do |num|
  print "#{(99.4708025861 * CMath.log(num) - 161.1195681661).round.clamp(0, 255)}, "
  count += 1
end
puts " };"
puts count
count = 0
print "uint8_t blues = { "
lower_range.step(upper_range, (upper_range - lower_range).to_f / 100) do |num|
  print "#{(138.5177312231 * CMath.log(num - 10) - 305.0447927307).round.clamp(0, 255)}, "
  count += 1
end
puts " };"
puts count