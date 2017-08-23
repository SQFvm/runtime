using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Discrod_Bot
{
    [XmlRoot("config")]
    public class ConfigXml
    {
        [XmlElement]
        public string Token { get; set; }

        public static ConfigXml Load(string path)
        {
            using (var stream = System.IO.File.OpenRead(path))
            {
                var serializer = new XmlSerializer(typeof(ConfigXml));
                return serializer.Deserialize(stream) as ConfigXml;
            }
        }
        public void Save(string path)
        {
            using (var stream = System.IO.File.Open(path, System.IO.FileMode.Create))
            {
                var serializer = new XmlSerializer(typeof(ConfigXml));
                serializer.Serialize(stream, this);
            }
        }
    }
}
