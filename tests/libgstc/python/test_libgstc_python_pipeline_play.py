#!/usr/bin/env python3
import unittest
import gstc

class TestGstcPipelinePlayMethods(unittest.TestCase):

    def test_libgstc_python_pipeline_play(self):
        pipeline = "videotestsrc name=v0 ! fakesink"
        self.gstd_client = gstc.client(loglevel='DEBUG')
        self.assertEqual(self.gstd_client.pipeline_create ("p0", pipeline), 0)
        self.assertEqual(self.gstd_client.pipeline_play ("p0"), 0)
        self.assertEqual(self.gstd_client.pipeline_stop ("p0"), 0)
        self.assertEqual(self.gstd_client.pipeline_delete ("p0"), 0)

if __name__ == '__main__':
    unittest.main()
