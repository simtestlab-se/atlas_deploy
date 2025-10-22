#!/bin/bash

# Quick EC2 Site Access Test Script
# Run this on your EC2 instance to test your MkDocs site

echo "🌐 MkDocs Site Access Test"
echo "=========================="

# Get public IP
PUBLIC_IP=$(curl -s ifconfig.me)
echo "📍 Public IP: $PUBLIC_IP"

# Test local nginx
echo ""
echo "🔧 Testing local nginx response:"
if curl -s -o /dev/null -w "%{http_code}" localhost | grep -q "200"; then
    echo "✅ Nginx responding locally (HTTP 200)"
else
    echo "❌ Nginx not responding properly"
fi

# Test site structure
echo ""
echo "📁 Site directory contents:"
if [ -d "/var/www/simtestlab" ]; then
    echo "✅ Site directory exists"
    echo "   Files: $(find /var/www/simtestlab -type f | wc -l)"
    echo "   Size: $(du -sh /var/www/simtestlab | cut -f1)"
    
    # Check main pages
    echo ""
    echo "📄 Main pages check:"
    [ -f "/var/www/simtestlab/index.html" ] && echo "   ✅ index.html" || echo "   ❌ index.html missing"
    [ -d "/var/www/simtestlab/bms_software" ] && echo "   ✅ bms_software/" || echo "   ❌ bms_software/ missing"
    [ -d "/var/www/simtestlab/blog" ] && echo "   ✅ blog/" || echo "   ❌ blog/ missing"
else
    echo "❌ Site directory /var/www/simtestlab does not exist"
fi

# Test from external perspective
echo ""
echo "🌍 External access test:"
echo "   Your site should be accessible at:"
echo "   http://$PUBLIC_IP"
echo ""

# Check security group (if AWS CLI is available)
if command -v aws >/dev/null 2>&1; then
    echo "🔒 Security group check:"
    # This would require AWS CLI configuration
    echo "   (Run 'aws ec2 describe-security-groups' to check port 80/443 access)"
else
    echo "🔒 Security group: Ensure ports 80 and 443 are open to 0.0.0.0/0"
fi

echo ""
echo "🚀 Next steps if site not accessible:"
echo "   1. Check security group allows HTTP (port 80)"
echo "   2. Verify nginx is running: sudo systemctl status nginx"
echo "   3. Check nginx config: sudo nginx -t"
echo "   4. View logs: sudo tail -f /var/log/nginx/error.log"

# Test a simple HTTP request
echo ""
echo "🧪 Sample page content:"
curl -s localhost | head -10 2>/dev/null || echo "Unable to fetch page content"