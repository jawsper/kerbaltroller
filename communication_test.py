import serial
import struct
import sys
import time
import logging
from collections import OrderedDict
import itertools

logging.basicConfig(level=logging.INFO, format='[%(asctime)s] %(levelname)s: %(message)s')

class Packet(OrderedDict):
	def __init__(self, fmt, labels, data=None):
		self.fmt = fmt
		self.labels = labels
		self.data = data if data else []

		super().__init__(zip(self.labels, self.data))

	def build(self):
		data = list(self.values())
		if not data:
			return
		
		packet_data = struct.pack(self.fmt, *data)
		packet = struct.pack('B', len(packet_data)) + packet_data
		checksum = 0
		for b in packet:
			checksum ^= b
		return b'\xBE\xEF' + packet + struct.pack('B', checksum)

	def parse(self, data):
		self.data = struct.unpack(self.fmt, data)
		self.update(zip(self.labels, self.data))



class HandshakePacket(Packet):
	def __init__(self):
		super().__init__('BBBB', [
			'id', 'M1', 'M2', 'M3'
		], b'\x01\x03\x01\x04')

class ControlPacket(Packet):
	def __init__(self, data=None):
		super().__init__('<BBBHBB9h', [
			'id', 'main_controls', 'mode', 'action_groups',
			'sas_navball_mode', 'additional_controls',
			'pitch', 'roll', 'yaw',
			'translate_x', 'translate_y', 'translate_z',
			'wheel_steer', 'throttle', 'wheel_throttle'
		], data)

class VesselDataPacket(Packet):
	def __init__(self, data=None):
		super().__init__('< B 8f 2i 2f I 5f 6f 6f 6f I 2f I4 f H 2B 2f 2B 2f B'.replace(' ', ''), [
			'id', 'AP', 'PE', 'SemiMajorAxis', 'SemiMinorAxis', 'VVI', 'e', 
			'inc', 'G', 'TAp', 'TPe', 'TrueAnomaly', 'Density', 'period', 'RAlt', 
			'Alt', 'Vsurf', 'Lat', 'Lon', 
			'LiquidFuelTot', 'LiquidFuel', 'OxidizerTot', 'Oxidizer', 'EChargeTot', 'ECharge', 
			'MonoPropTot', 'MonoProp', 'IntakeAirTot', 'IntakeAir', 'SolidFuelTot', 'SolidFuel', 
			'XenonGasTot', 'XenonGas', 'LiquidFuelTotS', 'LiquidFuelS', 'OxidizerTotS', 'OxidizerS', 
			'MissionTime', 'deltaTime', 'VOrbit', 'MNTime', 'MNDeltaV',
			'Pitch', 'Roll', 'Heading', 'action_groups', 
			'SOINumber', 'MaxOverHeat', 
			'MachNumber', 'IAS', 
			'CurrentStage', 'TotalStage', 
			'TargetDist', 'TargetV', 
			'NavballSASMode'
		], data)

class KerbalTroller:
	def __init__(self, port):
		self.port = serial.Serial(port, 38400, timeout=5)

	def make_packet(self, data):
		packet = b'\xBE\xEF' + struct.pack('B', len(data))
		checksum = len(data)
		for b in data:
			checksum ^= b
		return packet + data + struct.pack('B', checksum)

	def read_packet(self):
		start = self.port.read(1)
		while start != b'\xBE':
			start = self.port.read(1)
		if self.port.read(1) != b'\xEF':
			return False
		packet_len = struct.unpack('B', self.port.read(1))[0]
		packet = self.port.read(packet_len)
		checksum = struct.unpack('B', self.port.read(1))[0]

		calc_checksum = packet_len
		for b in packet:
			calc_checksum ^= b

		if checksum == calc_checksum:
			# logging.info(f'recv {packet}')
			return packet

		logging.info(f'{packet_len}, {packet}, {checksum}')
		logging.info(f'checksum failed: calculated {calc_checksum}, received {checksum}')
		return False


	def handshake(self):
		self.send(HandshakePacket().build())

	def read(self):
		return self.read_packet()

	def send(self, data):
		# logging.info(f'send {data}')
		self.port.write(data)

k = KerbalTroller(sys.argv[1])
time.sleep(1)
k.handshake()

def input_controls_to_output_controls(input_controls):
	# input: [7] SAS, [6] RCS, [5] Light, [4] Gears, [3] Brakes, [2] Precision, [1] Abort, [0] Stage
	# output: [0] SAS, [1] RCS, [2] Light, [3] Gears, [4] Brakes, [5] Abort, [6...15] Custom1...10
	output = 0
	inputbits = f'{input_controls:08b}'
	inputs = dict(zip(
		['SAS', 'RCS', 'lights', 'gears', 'brakes', 'precision', 'abort', 'stage'], 
		map(lambda x: x == '1', inputbits)
	))
	print(inputs)
	output_labels = [
		'SAS', 'RCS', 'lights', 'gears', 'brakes', 'abort', 'custom01', 'custom02',
		'custom03', 'custom04', 'custom05', 'custom06', 'custom07', 'custom08', 'custom09', 'custom10', 
	]
	for i, label in enumerate(output_labels):
		if inputs.get(label, False):
			output += 1 << i
	print(output)
	return output
	

vessel_data_list = [
	1,# [1] B packet_id
	0, # [ 2] f AP
	0, # [ 3] f PE
	0, # [ 4] f SemiMajorAxis
	0, # [ 5] f SemiMinorAxis
	0, # [ 6] f VVI
	0, # [ 7] f e
	0, # [ 8] f inc
	0, # [ 9] f G
	0, # [10] i TAp
	0, # [11] i TPe
	0, # [12] f TrueAnomaly
	0, # [13] f Density
	0, # [14] i period
	0, # [15] f RAlt
	0, # [16] f Alt
	0, # [17] f Vsurf
	0, # [18] f Lat
	0, # [19] f Lon
	0, # [20] f LiquidFuelTot
	0, # [21] f 
	0, # [22] f OxidizerTot
	0, # [23] f 
	0, # [24] f EChargeTot
	0, # [25] f 
	0, # [26] f MonoPropTot
	0, # [27] f 
	0, # [28] f IntakeAirTot
	0, # [29] f 
	0, # [30] f SolidFuelTot
	0, # [31] f 
	0, # [32] f XenonGasTot
	0, # [33] f 
	0, # [34] f LiquidFuelTotS
	0, # [35] f 
	0, # [36] f OxidizerTotS
	0, # [37] f 
	0, # [38] I MissionTime
	0, # [39] f deltaTime
	0, # [40] f VOrbit
	0, # [41] I MNTime
	0, # [42] f MNDeltaV
	0, # [43] f Pitch
	0, # [44] f Roll
	0, # [45] f Heading
	0, # [46] H ActionGroupStatus
	0, # [47] B SOINumber
	0, # [48] B MaxOverHeat
	0, # [49] f MachNumber
	0, # [50] f IAS
	0, # [51] B CurrentStage
	0, # [52] B TotalStage
	0, # [53] f TargetDist
	0, # [54] f TargetV
	0, # [55] B NavballSASMode
]

vessel_data = VesselDataPacket(vessel_data_list)
vessel_data['AP'] = 1000000
vessel_data['TAp'] = 0
vessel_data['PE'] = 100000
vessel_data['TPe'] = 1000
vessel_data['action_groups'] = 0x0
k.send(vessel_data.build())
vessel_data.dirty = True

dist_values = itertools.cycle([
	0, 500, 
	1000, 5000, 
	1000000, 5000000,
	1000000000
])

time_values = itertools.cycle([
	0, 30, 
	60, 1800, 
	3600, 60 * 60 * 24, 
	60 * 60 * 24 * 365
])

update_interval = 1

last_update = time.time()

while True:
	p = k.read()
	if p:
		pa = ControlPacket()
		pa.parse(p)
		output = input_controls_to_output_controls(pa['main_controls'])
		if output != vessel_data['action_groups']:
			vessel_data['action_groups'] = output
			vessel_data.dirty = True
	else:
		pass
		# print(p)

	if (time.time() - last_update) >= update_interval:
		vessel_data['AP'] = next(dist_values)
		vessel_data['TAp'] = next(time_values)
		last_update = time.time()
		vessel_data.dirty = True

	if True or vessel_data.dirty:
		vessel_data.dirty = False
		# print(vessel_data)
		k.send(vessel_data.build())