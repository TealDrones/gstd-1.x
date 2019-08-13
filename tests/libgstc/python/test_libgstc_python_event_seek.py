#!/usr/bin/env python3
import unittest
import gstc

class TestGstcEventSeekMethods(unittest.TestCase):
    def setUp(self):
        self.gstd_client = gstc.client(loglevel='DEBUG')

    def test_event_seek(self):
        pipeline = "videotestsrc name=v0 ! xvimagesink"
        self.assertEqual(self.gstd_client.pipeline_create ("p0", pipeline), 0)
        self.assertEqual(self.gstd_client.pipeline_play ("p0"), 0)
        self.assertEqual(self.gstd_client.event_seek("p0"), None)
        self.assertEqual(self.gstd_client.pipeline_stop ("p0"), 0)

if __name__ == '__main__':
    unittest.main()